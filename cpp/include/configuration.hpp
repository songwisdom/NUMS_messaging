#pragma once

#include <string>
#include <vector>
#include <shared_mutex>
#include <toml.hpp>   // 프로젝트에서 쓰는 toml 헤더 유지 (toml++ 계열)

namespace configuration {

class Configuration {
public:
    bool init(const std::string& path);

    std::string get_string(const std::string& section,
                           const std::string& key,
                           const std::string& def = "") const;

    int get_int(const std::string& section,
                const std::string& key,
                int def = 0) const;

    double get_float(const std::string& section,
                     const std::string& key,
                     double def = 0.0) const;

    bool get_bool(const std::string& section,
                  const std::string& key,
                  bool def = false) const;

    std::vector<std::string> get_array(const std::string& section,
                                       const std::string& key) const;

    std::vector<int> get_array_int(const std::string& section,
                                   const std::string& key,
                                   const std::vector<int>& def = {}) const;

    std::vector<toml::table> get_table_array(const std::string& section,
                                             const std::string& key) const;

private:
    mutable std::shared_mutex mu_;
    toml::table config_;
};

// ==============================
// Global (namespace) API
// ==============================
Configuration& global();

// 초기 로딩 + 경로 저장
bool init(const std::string& path);

// 현재 config 파일 경로
const std::string& config_path();

// 파일 변경 감지 후 필요 시 재로딩(변경 없으면 아무것도 안 함)
void do_realtime();

std::string get_string(const std::string& section,
                       const std::string& key,
                       const std::string& def = "");

int get_int(const std::string& section,
            const std::string& key,
            int def = 0);

double get_float(const std::string& section,
                 const std::string& key,
                 double def = 0.0);

bool get_bool(const std::string& section,
              const std::string& key,
              bool def = false);

std::vector<std::string> get_array(const std::string& section,
                                   const std::string& key);

std::vector<int> get_array_int(const std::string& section,
                               const std::string& key,
                               const std::vector<int>& def = {});

std::vector<toml::table> get_table_array(const std::string& section,
                                         const std::string& key);

} // namespace configuration
