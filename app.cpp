#include "spdlog/spdlog.h"

#include <iostream>

auto logger = spdlog::stdout_color_mt("main");

int main(int argc, char * argv[]) {
  logger->info("ran");
  exit(EXIT_SUCCESS);
}
