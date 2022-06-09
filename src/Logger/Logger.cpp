#include "Logger.h"
#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

std::string getCurrentDateTime() {
  time_t timer = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string output(30, '\0');
  std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", localtime(&timer));
  return output;
}

void Logger::Info(const std::string& message) {
  std::string output = "<INFO> [" + getCurrentDateTime() + "]" + message;
  std::cout << "\033[0;32m" << output << "\033[0m" << std::endl;
}

void Logger::Error(const std::string& message) {
  std::string output = "<ERROR> [" + getCurrentDateTime() + "]" + message;
  std::cerr << "\033[0;31m" << output << "\033[0m" << std::endl;
}
