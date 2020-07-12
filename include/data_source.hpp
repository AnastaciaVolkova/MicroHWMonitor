#ifndef CPU_FREQ_DATA_HPP_
#define CPU_FREQ_DATA_HPP__

#include <string>
#include <memory>

class DataSource
{
public:
    DataSource();
    virtual float GetValue() = 0;

protected:
    const std::string data_file_;
};

//! \brief Cpu frequency
class CpuFreqData : public DataSource
{
public:
    CpuFreqData();

    //! Get value of cpu frequency
    // \return cpu frequency
    float GetValue() override;
};

//! \brief Fam Rpm
class FanRpmData : public DataSource
{
public:
    FanRpmData();

    //! Get value of fan rpm
    // \return rpm
    float GetValue() override;
};

class DataSourceGenerator
{
public:
    //! Generate data source according to source name
    /*!
    \param source_name [in] name of source
    \return pointer to generated data source accourding to source_name or nullptr if source_name is invalid
    */
    static std::unique_ptr<DataSource> GetDataSource(std::string source_name);
};
#endif