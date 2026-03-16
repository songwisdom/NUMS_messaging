
#include "coroutine.h"
#include "server.hpp"


int main(int argc, char **argv) {
  init(argc, argv);
  
  std::uint16_t port = 9000;
    int threads = 4;

    Server server(port, threads);
    server.start();

  while (1) {
    spdlog::info("info log");
    spdlog::debug("debug log");
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}


