#include "LoggerMacros.hpp"

int main(){
  LOG_TRACE << "trace details";
  LOG_DEBUG << "debug data";
  LOG_INFO  << "something happened";
  LOG_WARN  << "suspicious situation";
  LOG_ERROR << "error text";
  LOG_FATAL << "fatal error";
}
