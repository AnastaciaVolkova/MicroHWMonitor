#include "data_source.hpp"
#include <fstream>
#include <string>

using std::ifstream;
using std::make_unique;
using std::stof;
using std::string;
using std::unique_ptr;

DataSource::DataSource(string data_file) : data_file_(data_file){};

CpuFreqData::CpuFreqData() : DataSource("/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq"){};

float CpuFreqData::GetValue()
{
  ifstream ifs(data_file_);
  string freq;
  ifs >> freq;
  return stof(freq);
};

FanRpmData::FanRpmData() : DataSource("/sys/class/hwmon/hwmon6/fan1_input"){};

float FanRpmData::GetValue()
{
  ifstream ifs(data_file_);
  string rpm;
  ifs >> rpm;
  return stof(rpm);
};

CpuTempData::CpuTempData() : DataSource("/sys/class/hwmon/hwmon0/temp1_input"){};

float CpuTempData::GetValue()
{
  ifstream ifs(data_file_);
  string rpm;
  ifs >> rpm;
  return stof(rpm);
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
    return make_unique<FanRpmData>();
  else if (source_name == "cpu_temp")
    return make_unique<CpuTempData>();
  else
    return nullptr;
};
