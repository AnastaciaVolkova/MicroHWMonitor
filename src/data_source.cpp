#include "data_source.hpp"
#include <fstream>
#include <string>

using std::ifstream;
using std::stof;
using std::string;

DataSource::DataSource() : data_file_{"/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq"} {};

CpuFreqData::CpuFreqData() : DataSource(){};

float CpuFreqData::GetValue()
{
  ifstream ifs("/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq");
  string freq;
  ifs >> freq;
  return stof(freq);
};