#pragma once
#include <variant>
#include <cstddef>
#include "dataElement.hpp"
#include <ctime>
#include <iostream>
#include <nlohmann/json.hpp>
#include "nums_constants.hpp"


namespace nums {
    using Body = std::variant<
        std::monostate, 
        sn_reginfo_body, 
        result_body
    >;

    class Packet {
    public:
        Header header;
        Body body{
            std::monostate{}
        };
        //int seq_num;

        Packet() = default;

        //LINK PACKET 생성자
        explicit Packet(MsgType t) {
            if (t == MsgType::LINK) {
                Header h{};
                h.set_msg_type(t);
                h.set_msg_len(t);
                h.set_seq_num(h.next_seq());
                header = h;
            }
        }

        std::string toString() const {
            return "HEADER:" + header.toString() +
                   "\nBODY:" + bodyToString();
        }

        std::string bodyToString() const {
            return std::visit([](auto&& b) -> std::string {
                using T = std::decay_t<decltype(b)>;
                if constexpr (std::is_same_v<T, std::monostate>) {
                    return "empty";
                } else {
                    return b.toString();  //각 body type별로 toString() 가지고 있음 
                }
            }, body);
        }

        std::size_t total_size() const { // Packet total size return
            return Header::SIZE + body_size();
        }
        
        
        static Packet parse_json(const std::byte* in, std::size_t n)
        { //json으로 들어오는 건 1,3 뿐임
            Packet p;
            std::string_view sv(reinterpret_cast<const char*>(in), n);
            auto j = nlohmann::json::parse(sv);
            //HEADER (msg_len 빼고 setting)
            p.header = Header::parse_json(in, n);
            //HEADER.msg_len, BODY
            switch (p.header.msg_type_enum()) {
                case MsgType::LINK:
                    p.header.set_msg_len(MsgType::LINK);
                    p.body = std::monostate{};
                    break;
                case MsgType::SN_REGINFO:
                    p.header.set_msg_len(MsgType::SN_REGINFO);
                    p.body = sn_reginfo_body::from_json(j);
                    break;
                default:
                    throw std::runtime_error("unknown msg_type");
            }
            return p;
        }

        

        template <std::size_t N>
        void read_array(std::array<char, N>& dst,
                        const std::byte* in,
                        std::size_t& offset){
            std::memcpy(dst.data(), in + offset, N);
            offset += N;
        }
        
        void serialize(std::byte* out, std::size_t out_size) const {
            const auto need = total_size();
            if (out_size < need) throw std::runtime_error("serialize: output buffer too small");
            std::size_t offset = 0;

            Header h = header;
            h.serialize(out);
            offset += Header::SIZE;

            std::visit([&](auto const& b) {
                using T = std::decay_t<decltype(b)>;

                if constexpr (std::is_same_v<T, std::monostate>) { // no body
                } else if constexpr (std::is_same_v<T, sn_reginfo_body> || std::is_same_v<T, result_body>) {
                    b.serialize(out + offset);
                } else {
                    throw std::runtime_error("serialize: unsupported body type");
                }
            }, body);
        }

        nlohmann::json to_json() const { // Packet 객체 확인해서 json 생성해주는 함수
            using json = nlohmann::json;
            json j;
            auto mt = header.get_msg_type();
            j["msg_type"] = std::string(mt.data(), strnlen(mt.data(), mt.size())); 
            if (std::holds_alternative<result_body>(body)) {
                const auto& b = std::get<result_body>(body);
                auto rc = b.get_result();

                std::string codeStr(rc.data(),strnlen(rc.data(), rc.size()));
                int code = std::stoi(codeStr); //문자열->int 변환
                //  result_code는 숫자지만 json에서는 문자열로 표현하기 위해서. 
                // 숫자 그대로 표현하려면 codeStr 대신 code 써도 됨

                j["result_code"] = codeStr;
                j["result_desc"] = result_desc(code); // Assuming the first character of result code represents the result type

            }
            return j;
            // j["action_code"] = header.action_code; //나중에 getter만들든가 말든가
            // j["seq_num"] = header.seq_num;
        }
        static std::string_view result_desc(int code) {
            switch(static_cast<RsltType>(code)) {
                case RsltType::OK: return "성공";
                case RsltType::SYSFAIL: return "시스템 실패";
                case RsltType::FORMAT_ERR: return "포맷 오류";
                case RsltType::ETC: return "기타 오류";
                case RsltType::INVALID_BODYLEN: return "잘못된 Body 길이";
                case RsltType::INVALID_CID: return "잘못된 CID";
                case RsltType::DUP_CIDNO: return "중복 CIDNO";
                case RsltType::DUPINSERT: return "중복 등록";
                case RsltType::DUPDELETE: return "중복 삭제";
                case RsltType::NOTROW: return "조건에 해당하는 Row 없음";
                case RsltType::INVALID_ACTION: return "잘못된 Action";
                default: return "UNKNOWN";
            }
        }
    
    private:
        std::size_t body_size() const {
            return std::visit([](auto&& b) -> std::size_t {
                using T = std::decay_t<decltype(b)>;
                if constexpr (std::is_same_v<T, std::monostate>)
                    return 0;
                else
                    return T::SIZE;
            }, body);
        }
    };
};
//packet 만들 때 msg_type 보고 Body 타입부터 정해서 Packet 생성
