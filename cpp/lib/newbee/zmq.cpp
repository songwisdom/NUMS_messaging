#include <zmq.hpp>
#include <string>
#include <iostream>

class ZmqServer {
public:
    ZmqServer()
        : ctx_(1),
          sock_(ctx_, zmq::socket_type::pull)
    {}

    void bind(const std::string& endpoint) {
        sock_.bind(endpoint);
        std::cout << "[ZMQ] bind: " << endpoint << "\n";
    }

    void run(){
        while (true) {
        zmq::message_t msg;
        // 2️⃣ recv
        auto ok = sock_.recv(msg, zmq::recv_flags::none);
        if (!ok) {
            std::cerr << "[ZMQ] recv failed\n";
            continue;
        }
        // raw bytes → string
        std::string payload(
            static_cast<char*>(msg.data()),
            msg.size()
        );
        // 3️⃣ validation
        if (!validate(payload)) {
            std::cerr << "[ZMQ] invalid msg\n";
            continue;
        }
        // 4️⃣ inq push
        inq_.push(payload);
        }
    }

private:
    zmq::context_t ctx_;
    zmq::socket_t  sock_;
};
