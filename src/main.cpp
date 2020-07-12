#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>
#include <fstream>

#include "reader.hpp"
#include "tinyxml2.h"
#include <stdexcept>

using std::ifstream;
using std::invalid_argument;
using std::string;

int main(int argc, char *argv[])
{
  Reader *reader;
  try
  {
    reader = new ReaderXML(argv[1]);
  }
  catch (invalid_argument exception)
  {
    std::cerr << exception.what() << std::endl;
    return -1;
  }
  string source;
  float hz;
  reader->GetParameters(source, hz);
  auto starts = std::chrono::system_clock::now();
  while (true)
  {
    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - starts).count() >= hz)
    {
      starts = std::chrono::system_clock::now();
      ifstream ifs("/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq");
      string freq;
      ifs >> freq;
      std::cout << freq << std::endl;
    }
  }
  return 0;
}
