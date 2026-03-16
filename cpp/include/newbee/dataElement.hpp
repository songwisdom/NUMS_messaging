#pragma once

#include <array>
#include <cstddef>
#include "nums_constants.hpp"
#include <nlohmann/json.hpp>


namespace nums {
    class Header { //mandatory
        private:
            std::array<char, 3> msg_type; //1,2,3,4,5,6
            std::array<char, 6> msg_len;
            std::array<char, 2> action_code;
            std::array<char, 13> seq_num; //규격서 변경됨(12->13)
        public:
            std::array<char, 3> get_msg_type() const;
            std::string toString() const;
            std::string bodyToString() const;
            MsgType msg_type_enum() const;
            static constexpr std::size_t SIZE = 24; //규격서 변경됨: 23->24
            Header();
            void set_msg_type(MsgType mt);
            void set_msg_len(MsgType mt); // Header + Body 포함 길이
            void serialize(std::byte* out) const;
            void deserialize(const std::byte* in);
            static Header parse_json(const std::byte* in, std::size_t n); //zmq_client 로부터 오는 JSON parsing -> header 생성
            static Header parse(const std::byte* in, std::size_t n);//smsc 로부터 오는 raw bytes parsing용
    };
    class sn_reginfo_body {
        private:
            std::array<char, 12> cid;
            std::array<char, 11> s_time;
            std::array<char, 11> e_time;
            std::array<char, 32> service_name;
            std::array<char, 11> info_rate; 
            std::array<char, 2> msg_type;
            std::array<char, 2> bill_type;
            std::array<char, 2> bill_limit;
            std::array<char, 3> reserverd_field_1;
        public:
            static constexpr std::size_t SIZE = 86;
            sn_reginfo_body();
            static sn_reginfo_body parse(const std::byte* in, std::size_t n);
            static sn_reginfo_body from_json(const nlohmann::json& j);
            void serialize(std::byte* out) const;
            std::string toString() const;
    };
    class result_body {
        private:
            std::array<char, 4> result;
        public:
            static constexpr std::size_t SIZE = 4;
            result_body();
            std::array<char, 4> get_result() const;
            static result_body parse(const std::byte* in, std::size_t n);
            void serialize(std::byte* out) const;
            void deserialize(const std::byte* in);
            std::string toString() const;
    };
}
      

//header의 msg_type 필드별 body format이 다름
//header.msg_type : SN_REGINFO
//header.msg_type : SN_REGINFO_ACK, REQ_RESULT, REQ_RESULT_ACK

//원본을 직접 보며(&), 읽기전용(const)
// //의미적으로 “값”인 경우는 굳이 & 까지 필요 x

//밑 둘 똑같은 의미
//Packet() = default;
//Packet() : body(std::monostate{}) {}