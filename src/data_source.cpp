#include "data_source.hpp"
#include <fstream>
#include <string>

using std::ifstream;
using std::make_unique;
using std::stof;
using std::string;
using std::unique_ptr;

DataSource::DataSource() : data_file_{"/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq"} {};

CpuFreqData::CpuFreqData() : DataSource(){};

float CpuFreqData::GetValue()
{
  ifstream ifs("/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq");
  string freq;
  ifs >> freq;
  return stof(freq);
};

unique_ptr<DataSource> DataSourceGenerator::GetDataSource(string source_name)
{
  while (source_name[0] == ' ')
    source_name.erase(' ');
  while (source_name[source_name.size() - 1] == ' ')
    source_name.erase(source_name.size() - 1);
  if (source_name == "cpu")
    return make_unique<CpuFreqData>();
  else if (source_name == "fan")
    return nullptr;
  else
    return nullptr;
};
