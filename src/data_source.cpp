#include "data_source.hpp"
#include <fstream>
#include <string>
#include <stdexcept>

using std::ifstream;
using std::istringstream;
using std::make_unique;
using std::map;
using std::stof;
using std::string;
using std::unique_ptr;

DataSource::DataSource(string data_file, bool no_data) : data_file_(data_file), no_data_(no_data){};

CpuFreqData::CpuFreqData(string file_name) : DataSource(file_name, false){};

float CpuFreqData::GetValue()
{
  ifstream ifs(data_file_);
  string freq;
  ifs >> freq;
  return stof(freq);
};

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

map<string, string> DataSourceGenerator::sensor_files_{
    {"cpu", "/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq"},
    {"fan", "/sys/class/hwmon/hwmon6/fan1_input"},
    {"cpu_temp", "/sys/class/hwmon/hwmon8/temp1_input"},
};

unique_ptr<DataSource> DataSourceGenerator::GetDataSource(string source_name)
{
  while (source_name[0] == ' ')
    source_name.erase(' ');
  while (source_name[source_name.size() - 1] == ' ')
    source_name.erase(source_name.size() - 1);

  if (source_name == "")
    return make_unique<SavedData>(source_name);

  if (sensor_files_.find(source_name) == sensor_files_.end())
    return nullptr;

  return make_unique<CpuFreqData>(sensor_files_[source_name]);
};
