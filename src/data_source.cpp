#include "data_source.hpp"
#include <fstream>
#include <string>
#include <stdexcept>

using std::ifstream;
using std::istringstream;
using std::make_unique;
using std::stof;
using std::string;
using std::unique_ptr;

DataSource::DataSource(string data_file, bool no_data) : data_file_(data_file), no_data_(no_data){};

CpuFreqData::CpuFreqData() : DataSource("/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq", false){};

float CpuFreqData::GetValue()
{
  ifstream ifs(data_file_);
  string freq;
  ifs >> freq;
  return stof(freq);
};

FanRpmData::FanRpmData() : DataSource("/sys/class/hwmon/hwmon6/fan1_input", false){};

float FanRpmData::GetValue()
{
  ifstream ifs(data_file_);
  string rpm;
  ifs >> rpm;
  return stof(rpm);
};

CpuTempData::CpuTempData() : DataSource("/sys/class/hwmon/hwmon0/temp1_input", false){};

float CpuTempData::GetValue()
{
  ifstream ifs(data_file_);
  string rpm;
  ifs >> rpm;
  return stof(rpm);
};
#include <iostream>
SavedData::SavedData(string file_name) : DataSource(file_name, false)
{
  ifs_.open(file_name);
};

float SavedData::GetValue()
{
  string val;
  if (!getline(iss_, val, ' '))
  {
    if (!getline(ifs_, val))
    {
      no_data_ = true;
      return 0;
    }
    else
    {
      if (val == " ")
      {
        no_data_ = true;
        return 0;
      }
      iss_ = istringstream(val);
      getline(iss_, val, ' ');
    }
  }
  return stof(val);
}

SavedData::~SavedData()
{
  ifs_.close();
}

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
  else if (source_name != "")
    return make_unique<SavedData>(source_name);
  else
    return nullptr;
};
