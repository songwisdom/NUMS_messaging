#include "configuration.hpp"

#include <sstream>
#include <mutex>
#include <shared_mutex>   // std::shared_lock, std::unique_lock
#include <filesystem>
#include <system_error>

namespace configuration {

namespace { // configuration.cpp 내부 전용 헬퍼

std::vector<std::string> split_section(const std::string& section) {
    std::vector<std::string> out;
    std::stringstream ss(section);
    std::string token;
    while (std::getline(ss, token, '.')) {
        if (!token.empty())
            out.push_back(token);
    }
    return out;
}

// node* 대신 node_view 반환 (value_or 사용 가능)
toml::node_view<const toml::node> find_node_view(const toml::table& root,
                                                 const std::string& section,
                                                 const std::string& key) {
    const toml::table* tbl = &root;

    // section 경로 따라가기
    for (const auto& s : split_section(section)) {
        auto it = tbl->find(s);
        if (it == tbl->end())
            return {}; // empty view

        auto next_tbl = it->second.as_table();
        if (!next_tbl)
            return {}; // empty view

        tbl = next_tbl;
    }

    // 최종 key 반환
    return (*tbl)[key]; // node_view
}

} // anonymous namespace

// ==============================
// Configuration (local instance)
// ==============================

bool Configuration::init(const std::string& path) {
    try {
        toml::table t = toml::parse_file(path);
        std::unique_lock lock(mu_);
        config_ = std::move(t);
        return true;
    } catch (...) {
        return false;
    }
}

std::string Configuration::get_string(const std::string& section,
                                      const std::string& key,
                                      const std::string& def) const {
    std::shared_lock lock(mu_);
    auto n = find_node_view(config_, section, key);

    auto v = n.value_or(def);
    if (v.empty()) return def;   // 빈 문자열이면 def 유지
    return v;
}

int Configuration::get_int(const std::string& section,
                           const std::string& key,
                           int def) const {
    std::shared_lock lock(mu_);
    auto n = find_node_view(config_, section, key);
    return n.value_or(def);
}

double Configuration::get_float(const std::string& section,
                                const std::string& key,
                                double def) const {
    std::shared_lock lock(mu_);
    auto n = find_node_view(config_, section, key);

    // float은 int로 적어놔도 읽히게
    if (auto v = n.value<double>())
        return *v;
    if (auto v2 = n.value<int64_t>())
        return static_cast<double>(*v2);

    return def;
}

bool Configuration::get_bool(const std::string& section,
                             const std::string& key,
                             bool def) const {
    std::shared_lock lock(mu_);
    auto n = find_node_view(config_, section, key);
    return n.value_or(def);
}

std::vector<std::string> Configuration::get_array(const std::string& section,
                                                  const std::string& key) const {
    std::vector<std::string> out;

    std::shared_lock lock(mu_);
    auto n = find_node_view(config_, section, key);

    auto arr = n.as_array();
    if (!arr) return out;

    out.reserve(arr->size());
    for (auto& elem : *arr) {
        auto s = elem.value_or<std::string>("");
        if (!s.empty())
            out.push_back(std::move(s));
    }
    return out;
}

std::vector<int> Configuration::get_array_int(const std::string& section,
                                              const std::string& key,
                                              const std::vector<int>& def) const {
    std::vector<int> out;

    std::shared_lock lock(mu_);
    auto n = find_node_view(config_, section, key);

    auto arr = n.as_array();
    if (!arr) return def;

    out.reserve(arr->size());
    for (auto& elem : *arr) {
        if (auto v = elem.value<int64_t>())
            out.push_back(static_cast<int>(*v));
    }

    if (out.empty())
        return def;

    return out;
}

std::vector<toml::table> Configuration::get_table_array(const std::string& section,
                                                        const std::string& key) const {
    std::vector<toml::table> out;
    std::shared_lock lock(mu_);

    auto arr = find_node_view(config_, section, key).as_array();
    if (!arr) return out;

    for (const auto& e : *arr) {
        if (auto tbl = e.as_table())
            out.push_back(*tbl);
    }
    return out;
}

// ==============================
// Global (namespace) instance + realtime reload
// ==============================

static Configuration g_config;

// realtime 감지용 상태
namespace fs = std::filesystem;

static std::string g_config_path;
static std::mutex g_reload_mu;
static fs::file_time_type g_last_mtime{};
static bool g_mtime_inited = false;

Configuration& global() {
    return g_config;
}

bool init(const std::string& path) {
    g_config_path = path;
    g_mtime_inited = false; // init 다시 하면 기준도 리셋
    return global().init(path);
}

const std::string& config_path() {
    return g_config_path;
}

void do_realtime() {
    if (g_config_path.empty())
        return; // init 안됨

    std::lock_guard<std::mutex> lk(g_reload_mu);

    std::error_code ec;
    auto mt = fs::last_write_time(g_config_path, ec);
    if (ec) {
        // stat 실패(파일 없음/권한 등) → 기존 config 유지
        return;
    }

    // 최초 1회 기준 설정
    if (!g_mtime_inited) {
        g_last_mtime = mt;
        g_mtime_inited = true;
        return;
    }

    // 변경 없음
    if (mt == g_last_mtime)
        return;

    // 변경됨 → reload
    if (global().init(g_config_path)) {
        g_last_mtime = mt;
    } else {
        // reload 실패 시 기존 config 유지
    }
}

// 전역 편의 함수들 (global()로 forward)
std::string get_string(const std::string& section,
                       const std::string& key,
                       const std::string& def) {
    return global().get_string(section, key, def);
}

int get_int(const std::string& section,
            const std::string& key,
            int def) {
    return global().get_int(section, key, def);
}

double get_float(const std::string& section,
                 const std::string& key,
                 double def) {
    return global().get_float(section, key, def);
}

bool get_bool(const std::string& section,
              const std::string& key,
              bool def) {
    return global().get_bool(section, key, def);
}

std::vector<std::string> get_array(const std::string& section,
                                   const std::string& key) {
    return global().get_array(section, key);
}

std::vector<int> get_array_int(const std::string& section,
                               const std::string& key,
                               const std::vector<int>& def) {
    return global().get_array_int(section, key, def);
}

std::vector<toml::table> get_table_array(const std::string& section,
                                         const std::string& key) {
    return global().get_table_array(section, key);
}

} // namespace configuration
