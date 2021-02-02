#include "include/ThostFtdcMdApi.h"
#include "MdSpi.h"
#include <iostream>

char* MD_FRONT = "tcp://180.168.146.187:10110"; // SimNow

int main(int argc, char **argv)
{
  std::cout << "hello, world!" << std::endl;

  CThostFtdcMdApi *api = CThostFtdcMdApi::CreateFtdcMdApi("tmp/md", false);
  MdSpi spi(api);
  api->RegisterSpi(&spi);
  api->RegisterFront(MD_FRONT);
  api->Init();
  api->Join();
  return 0;
}
