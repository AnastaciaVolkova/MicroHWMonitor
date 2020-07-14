#ifndef CPU_FREQ_DATA_HPP_
#define CPU_FREQ_DATA_HPP__

#include <string>
#include <memory>
#include <fstream>
#include <sstream>

class DataSource
{
public:
    DataSource(std::string data_file, bool no_data);
    virtual ~DataSource(){};
    virtual float GetValue() = 0;
    bool NoData() { return no_data_; };

protected:
    const std::string data_file_;
    bool no_data_;
};

//! \brief Cpu frequency
class CpuFreqData : public DataSource
{
public:
    //! \param [in] file_name name of file, which corresponds to current sensor
    CpuFreqData(std::string file_name);

    //! Get value of cpu frequency
    // \return cpu frequency
    float GetValue() override;
};

//! \brief Saved data
class SavedData : public DataSource
{
private:
    std::ifstream ifs_;
    std::istringstream iss_;

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