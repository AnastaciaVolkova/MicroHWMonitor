#ifndef CPU_FREQ_DATA_HPP_
#define CPU_FREQ_DATA_HPP__

#include <string>

class DataSource
{
public:
    DataSource();
    virtual float GetValue() = 0;

protected:
    const std::string data_file_;
};

class CpuFreqData : public DataSource
{
public:
    CpuFreqData();
    float GetValue() override;
};

#endif