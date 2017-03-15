#include "iopm_client.hpp"

namespace {
class logger final {
public:
  static spdlog::logger& get_logger(void) {
    static std::shared_ptr<spdlog::logger> logger;
    if (!logger) {
      logger = spdlog::stdout_logger_mt("iopmlib", true);
    }
    return *logger;
  }
};
}

int main(int argc, const char* argv[]) {
  krbn::thread_utility::register_main_thread();

  krbn::iopm_client client(logger::get_logger(), [](uint32_t message_type) {
    logger::get_logger().info("callback message_type:{0}", message_type);
  });

  CFRunLoopRun();

  return 0;
}
