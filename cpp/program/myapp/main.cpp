#include "common.h"

int main(int argc, char **argv) {
  init(argc,argv);
 
  while (1) {
    spdlog::info("info log");
    spdlog::debug("debug log");
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
