#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>
#include <fstream>

#include "tinyxml2.h"

using std::string;
using std::ifstream;

int main(int argc, char *argv[])
{
  tinyxml2::XMLDocument doc;
  doc.LoadFile(argv[1]);
  tinyxml2::XMLElement *element = doc.RootElement();
  string source = doc.RootElement()->FirstChildElement("source")->GetText();
  float hz = std::stof(doc.RootElement()->FirstChildElement("hz")->GetText());
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
