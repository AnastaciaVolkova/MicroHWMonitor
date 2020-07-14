#ifndef CPU_FREQ_DATA_HPP_
#define CPU_FREQ_DATA_HPP__

#include <string>
#include <memory>
#include <fstream>
class DataSource
{
public:
    DataSource(std::string data_file);
    virtual ~DataSource(){};
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

//! \brief Fan Rpm
class FanRpmData : public DataSource
{
public:
    FanRpmData();

    //! Get value of fan rpm
    // \return rpm
    float GetValue() override;
};

//! \brief Cpu Temp
class CpuTempData : public DataSource
{
public:
    CpuTempData();

    //! Get value of cpu temperature
    // \return cpu temperature
    float GetValue() override;
};

//! \brief Saved data
class SavedData : public DataSource
{
private:
    std::ifstream ifs_;

public:
    SavedData(std::string file_name);
    ~SavedData() override;

    //! Get value of saved data
    // \return sample of saved data
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