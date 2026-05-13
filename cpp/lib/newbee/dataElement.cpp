#include <iostream>
#include <array>
#include <cstddef> 
#include <algorithm>
#include <cstring>
#include <span>
#include <charconv>
#include <system_error>
#include <stdexcept>
#include "nums_constants.hpp"
#include "dataElement.hpp"
//JSON Parsing
#include <nlohmann/json.hpp>
#include <atomic>

namespace nums {
    Header::Header(){ 
        msg_type.fill('\0');
        msg_len.fill('\0');
        action_code.fill('\0');
        seq_num.fill('\0');
    }

    std::array<char, 3> Header::get_msg_type() const{
        return msg_type;
    }
    size_t Header::get_msg_len() const{
        std::string len(msg_len.data(), msg_len.size());
        return std::stoul(len);
    }
    
    // 밑 함수 - 만약 MsgType 가짓수가 적다면 굳이스럽지 않음?
    MsgType Header::msg_type_enum() const {
        if (std::memcmp(msg_type.data(), "1\0\0", 3) == 0) return MsgType::LINK;
        if (std::memcmp(msg_type.data(), "2\0\0", 3) == 0) return MsgType::LINK_ACK;
        if (std::memcmp(msg_type.data(), "3\0\0", 3) == 0) return MsgType::SN_REGINFO;
        if (std::memcmp(msg_type.data(), "4\0\0", 3) == 0) return MsgType::SN_REGINFO_ACK;
        if (std::memcmp(msg_type.data(), "5\0\0", 3) == 0) return MsgType::REQ_RESULT;
        if (std::memcmp(msg_type.data(), "6\0\0", 3) == 0) return MsgType::REQ_RESULT_ACK;
        return MsgType::UNKNOWN;
    }

    template <std::size_t N>
    void read_array(std::array<char, N>& dst,
                    const std::byte* in,
                    std::size_t& offset){
        std::memcpy(dst.data(), in + offset, N);
        offset += N;
    }

    //seq_num 증가처리
    std::atomic<uint64_t> Header::seq{1}; //정의

    std::string Header::next_seq() {
        return std::to_string(seq++);
    }
    void Header::set_seq_num(const std::string& seq){
        std::memcpy(
            seq_num.data(),
            seq.data(),
            std::min(seq.size(), seq_num.size())
        );
    }

    Header Header::parse_json(const std::byte* in, std::size_t n){
        Header h{}; // 생성자에 fill('\0') 있음
        std::string_view sv(reinterpret_cast<const char*>(in), n); //복사 x, 메모리 재할당 x -> string_view
        auto j = nlohmann::json::parse(sv);
        //header - msg_type
        std::string tmp = j.at("msg_type").get<std::string>();
        std::memcpy(
            h.msg_type.data(),
            tmp.data(),
            std::min(tmp.size(), h.msg_type.size())
        );
        
        //header - action_code
        std::string tmp2 = j.at("action_code").get<std::string>();
        std::memcpy(
            h.action_code.data(),
            tmp2.data(),
            std::min(tmp2.size(), h.action_code.size())
        );

        std::string tmp3 = h.next_seq();
        std::memcpy(
            h.seq_num.data(),
            tmp3.data(),
            std::min(tmp3.size(), h.seq_num.size())
        );
        return h;
    }

    Header Header::parse(const std::byte* in, std::size_t n){
        if(n < Header::SIZE){
            throw std::runtime_error("not enough data for Header");
        }
        Header h;
        std::size_t offset = 0;
        read_array(h.msg_type, in, offset); 
        read_array(h.msg_len, in, offset);
        read_array(h.action_code, in, offset);
        read_array(h.seq_num, in, offset);
        return h;
    }
    
    void Header::set_msg_type(MsgType mt){
        int value = static_cast<int>(mt);
        //int 값. msg_type은 std::array<char, 3> 이고 변환 필요
        auto [ptr, ec] = std::to_chars(
            msg_type.data(),
            msg_type.data() + msg_type.size(),
            value
        );
        if (ec != std::errc{}) {
            throw std::runtime_error("msg_type overflow");
        }
    }

    void Header::set_msg_len(MsgType mt){ 
        size_t bodysize=0;
        switch(mt){
            case MsgType::LINK:
            case MsgType::LINK_ACK: bodysize = 0; break;
            case MsgType::SN_REGINFO: bodysize = sn_reginfo_body::SIZE; break;
            case MsgType::SN_REGINFO_ACK:
            case MsgType::REQ_RESULT:
            case MsgType::REQ_RESULT_ACK: bodysize = result_body::SIZE; break;
            default: bodysize = 0;
        }
        std::string s = std::to_string(bodysize);
        std::memcpy(
            msg_len.data(),
            s.data(),
            std::min(s.size(), msg_len.size())
        );
    }
    
    void Header::serialize(std::byte* out) const { 
        //const: 객체 변경 하지 않겠다
        std::size_t offset = 0;
        std::memcpy(out + offset, msg_type.data(), msg_type.size());
        offset += msg_type.size();
        std::memcpy(out + offset, msg_len.data(), msg_len.size());
        offset += msg_len.size();
        std::memcpy(out + offset, action_code.data(), action_code.size());
        offset += action_code.size();
        std::memcpy(out + offset, seq_num.data(), seq_num.size());
    }

    void Header::deserialize(const std::byte* in) {
        std::size_t offset = 0;
        std::memcpy(msg_type.data(), in + offset, msg_type.size());
        offset += msg_type.size();
        std::memcpy(msg_len.data(), in + offset, msg_len.size());
        offset += msg_len.size();
        std::memcpy(action_code.data(), in + offset, action_code.size());
        offset += action_code.size();
        std::memcpy(seq_num.data(), in + offset, seq_num.size());
    }

    static std::string arr_to_string(const auto& arr) {
        std::string s(arr.begin(), arr.end());
        auto pos = s.find('\0');
        if (pos != std::string::npos) {
            s.erase(pos);
        }
        return s;
    }
    
    std::string Header::toString() const{
        return "Header{"
        "msg_type=" + arr_to_string(msg_type) +
        ", msg_len=" + arr_to_string(msg_len) +
        ", action_code=" + arr_to_string(action_code) +
        ", seq_num=" + arr_to_string(seq_num) +
        "}";
    }

    sn_reginfo_body::sn_reginfo_body(){
        cid.fill('\0');
        s_time.fill('\0');
        e_time.fill('\0');
        service_name.fill('\0');
        info_rate.fill('\0');
        msg_type.fill('\0');
        bill_type.fill('\0');
        bill_limit.fill('\0');
        reserverd_field_1.fill('\0');
    }
     sn_reginfo_body sn_reginfo_body::from_json(const nlohmann::json& j){
        sn_reginfo_body b;
        auto copy = [](auto& arr, const std::string& s){
            std::fill(arr.begin(), arr.end(), '\0');
            std::memcpy(arr.data(),
            s.data(),
            std::min(arr.size(), s.size()));};
        copy(b.cid,          j["cid"].get<std::string>());
        copy(b.s_time,       j["s_time"].get<std::string>());
        copy(b.e_time,       j["e_time"].get<std::string>());
        copy(b.service_name, j["service_name"].get<std::string>());
        copy(b.info_rate,    j["info_rate"].get<std::string>());
        copy(b.msg_type,     j["body_msg_type"].get<std::string>());
        copy(b.bill_type,    j["bill_type"].get<std::string>());
        copy(b.bill_limit,   j["bill_limit"].get<std::string>());
        return b;
    }

    std::string sn_reginfo_body::toString() const {
        auto to_str = [](const auto& arr) {
            return std::string(arr.data(), arr.size());
        };
        std::ostringstream oss;
        oss << "cid=" << to_str(cid)
            << ", s_time=" << to_str(s_time)
            << ", e_time=" << to_str(e_time)
            << ", service_name=" << to_str(service_name)
            << ", info_rate=" << to_str(info_rate)
            << ", msg_type=" << to_str(msg_type)
            << ", bill_type=" << to_str(bill_type)
            << ", bill_limit=" << to_str(bill_limit);
        return oss.str();
    }
    result_body::result_body() {
        result.fill('\0');
    }
    std::string result_body::toString() const {
        return "Body{"
        "result=" + arr_to_string(result) +
        "}";
    }
    //body type 별 serialize 
    template <std::size_t N>
    void write_array(const std::array<char, N>& src,
                    std::byte* out,
                    std::size_t& offset){
        std::memcpy(out + offset, src.data(), N);
        offset += N;
    }
    void sn_reginfo_body::serialize(std::byte* out) const{
        std::size_t offset = 0;
        write_array(cid, out, offset);
        write_array(s_time, out, offset);
        write_array(e_time, out, offset);
        write_array(service_name, out, offset);
        write_array(info_rate, out, offset);
        write_array(msg_type, out, offset);
        write_array(bill_type, out, offset);
        write_array(bill_limit, out, offset);
        write_array(reserverd_field_1, out, offset);
    }

    std::array<char, 4> result_body::get_result() const{
        return result;
    }

    void result_body::serialize(std::byte* out) const{
        std::size_t offset = 0;
        write_array(result, out, offset); 
    }
    void result_body::deserialize(const std::byte* in) {
        std::memcpy(result.data(), in, result.size());
    }

    sn_reginfo_body sn_reginfo_body::parse(const std::byte* in, std::size_t n){
        sn_reginfo_body s;
        std::size_t offset = 0;
        read_array(s.cid, in, offset); 
        read_array(s.s_time, in, offset);
        read_array(s.e_time, in, offset);
        read_array(s.service_name, in, offset);
        read_array(s.info_rate, in, offset);
        read_array(s.msg_type, in, offset);
        read_array(s.bill_type, in, offset);
        read_array(s.bill_limit, in, offset);
        read_array(s.reserverd_field_1, in, offset);
        return s;
    }

    result_body result_body::parse(const std::byte* in, std::size_t n){
        result_body r;
        std::size_t offset = 0;
        read_array(r.result, in, offset); 
        return r;
    }
};

//const std::array<char, 3>& mt, const std::array<char, 2>& ac

/*
void Header::serialize(std::byte* out) const{
        static constexpr std::size_t SIZE = 23;
        auto dst = std::span<std::byte, SIZE>(out, SIZE);
        std::size_t offset = 0;

        auto copy_field = [&](const auto& field) {
        std::copy_n(
            reinterpret_cast<const std::byte*>(field.data()),
            field.size(),
            dst.begin() + offset
        );
        offset += field.size();
        };

        copy_field(msg_type);
        copy_field(msg_len);
        copy_field(action_code);
        copy_field(seq_num);
    }
    
    */
    /*
    #define E_SYSFAIL 1 // 시스템 장애
    #define E_FORMAT_ERR 2 // 메세지 형식 오류
    #define E_ETC 3 // 기타
    #define SN_REGINFO_ACK_E_OK 0 // 성공
    #define SN_REGINFO_ACK_E_INVALID_BODYLEN 14 // Header의 메시지길이와 Body사이즈 불일치
    #define SN_REGINFO_ACK_E_INVALID_CID 15 // Invalid CID Length
    #define REQ_RESULT_E_OK 0 // SN_REGINFO, SUBCID, POLL Action 성공
    #define REQ_RESULT_E_DUP_CIDNO 5 //중복 CID No 존재
    #define REQ_RESULT_E_DUPINSERT 10 //등록 에러
    #define REQ_RESULT_E_DUPDELETE 11 // 삭제 에러
    #define REQ_RESULT_E_NOTROW 12 // 조건에 해당하는 Row없음
    #define REQ_RESULT_E_INVALID_ACTION 16 // Invalid Action Code
    */



/* 이전 방식 이건 뭘까?
    auto [ptr, ec] = std::to_chars(
        msg_len.data(),
        msg_len.data() + msg_len.size(),
        len
    );
    if (ec != std::errc{})
        throw std::runtime_error("msg_len overflow");
*/