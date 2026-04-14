#include "zmq_client.hpp"
using namespace rapidjson;

using namespace std;

zmq_client::zmq_client() = default;

zmq_client::~zmq_client()
{
    stop(); 
}

void zmq_client::stop()
{
    stop_.store(true);

    // monitor thread
    if (mon_th_.joinable()) {
        mon_th_.join();
    }
}

void zmq_client::input_msg(nums_msg &msg, StringBuffer &s)
{
    this_thread::sleep_for(chrono::seconds(1));
    std::string input;

    std::memset(&msg, 0, sizeof(msg));

#if 0
    std::snprintf(msg.msg_type, sizeof(msg.msg_type), "%s", "3");
    std::snprintf(msg.action_code, sizeof(msg.action_code), "%s", "A");
    std::snprintf(msg.cid, sizeof(msg.cid), "%s", "#0493");
    std::snprintf(msg.s_time, sizeof(msg.s_time), "%s", "2026010900");
    std::snprintf(msg.e_time, sizeof(msg.e_time), "%s", "2026010924");
    std::snprintf(msg.service_name, sizeof(msg.service_name), "%s", "TEST");
    std::snprintf(msg.info_rate, sizeof(msg.info_rate), "%s", "100");
    std::snprintf(msg.body_msg_type, sizeof(msg.body_msg_type), "%s", "1");
    std::snprintf(msg.bill_type, sizeof(msg.bill_type), "%s", "2");
    std::snprintf(msg.bill_limit, sizeof(msg.bill_limit), "%s", "3");
#else
    std::cout << "msg_type: 1 LINK, 3 REQ, 5 RES -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.msg_type, sizeof(msg.msg_type), "%s", input.c_str());

    std::cout << "action_code: A, U, D -> " ;
    std::getline(std::cin, input);
    std::snprintf(msg.action_code, sizeof(msg.action_code), "%s", input.c_str());

    std::cout << "cid: #0493 -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.cid, sizeof(msg.cid), "%s", input.c_str());
std::cout << "s_time: YYYYMMDDHH -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.s_time, sizeof(msg.s_time), "%s", input.c_str());

    std::cout << "e_time: YYYYMMDDHH -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.e_time, sizeof(msg.e_time), "%s", input.c_str());

    std::cout << "service_name: TEST -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.service_name, sizeof(msg.service_name), "%s", input.c_str());

    std::cout << "info_rate: 100 -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.info_rate, sizeof(msg.info_rate), "%s", input.c_str());

    std::cout << "body_msg_type: 1 -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.body_msg_type, sizeof(msg.body_msg_type), "%s", input.c_str());

    std::cout << "bill_type: 2 -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.bill_type, sizeof(msg.bill_type), "%s", input.c_str());

    std::cout << "bill_limit: 3 -> ";
    std::getline(std::cin, input);
    std::snprintf(msg.bill_limit, sizeof(msg.bill_limit), "%s", input.c_str());
#endif

    PrettyWriter<StringBuffer> writer(s);
    writer.StartObject();

    writer.Key(NUMS_NAME_MSG_TYPE);
    writer.String(msg.msg_type);

    writer.Key(NUMS_NAME_ACTION_CODE);
    writer.String(msg.action_code);

    writer.Key(NUMS_NAME_CID);
    writer.String(msg.cid);

    writer.Key(NUMS_NAME_S_TIME);
    writer.String(msg.s_time);

    writer.Key(NUMS_NAME_E_TIME);
    writer.String(msg.e_time);

    writer.Key(NUMS_NAME_SVC_NAME);
    writer.String(msg.service_name);
    writer.Key(NUMS_NAME_INFO_RATE);
    writer.String(msg.info_rate);

    writer.Key(NUMS_NAME_BODY_MSG_TYPE);
    writer.String(msg.body_msg_type);

    writer.Key(NUMS_NAME_BILL_TYPE);
    writer.String(msg.bill_type);

    writer.Key(NUMS_NAME_BILL_LIMIT);
    writer.String(msg.bill_limit);

    writer.EndObject();
}

void zmq_client::start(int idx)
{
    spdlog::info("zmq client start");
    monitor_.init(sock_, fmt::format("inproc://zmqcli-monitor-{}",idx)); //프로세스 간 통신을 위해서는 inproc->ipc, 네트워크 간: tcp

    mon_th_ = std::thread([this] {
        while (!stop_.load()) {
            (void)monitor_.check_event(100);
        }
    });

    sock_.set(zmq::sockopt::linger, 0);
    sock_.set(zmq::sockopt::sndhwm, 30000);

    const std::string conn_addr = "tcp://127.0.0.1:9984"; //local port
    sock_.connect(conn_addr);

    while (!stop_.load()) {
        nums_msg msg;
        StringBuffer s;

        input_msg(msg, s);

        std::cout << "msg : " << s.GetString() << std::endl;
        const auto ret = sock_.send(zmq::buffer(s.GetString(),s.GetSize()), zmq::send_flags::none);
        
        if (!ret) {
            const int err = zmq_errno();
            spdlog::error("[{}:{}] send failed: errno={} ({}) buf[{}]",__func__, __LINE__, err, zmq_strerror(err), s.GetString());
        } else {
            spdlog::info("[{}:{}] send ok: bytes={} buf[{}]",__func__, __LINE__, *ret, s.GetString());
        }

        zmq::message_t reply;
        const auto rc = sock_.recv(reply, zmq::recv_flags::none); //블로킹!!! 계속 기다리는 recv아냐? 논블로킹은 epoll (2026-02-22)
        if(!rc){
            std::cout << "recv fail " << std::endl;
        }

        std::string reply_msg(static_cast<char*>(reply.data()), reply.size());
        std::cout << "recv:" << reply_msg << std::endl;
    }
    spdlog::info("zmq client stop");
}
