#ifndef READER_HPP_
#define READER_HPP_

#include <string>
#include <vector>
#include <fstream>
#include "tinyxml2.h"
#ifdef MULTI_TH
#include <mutex>
#include <condition_variable>
#include <queue>
#endif
class Reader
{
public:
  //! Get source and frequency from input parameters
  // \param [out] source source (cpu, fan etc)
  // \param [out] hz frequency (Hz)
  // \param [out] transform transform to apply
  virtual void GetParameters(std::string &source, float &hz, std::string &transform) = 0;
};

//! \brief Stands for reading data from xml file
class ReaderXML : public Reader
{
private:
  tinyxml2::XMLDocument doc_;

public:
  ReaderXML(const std::string &in_file);
  void GetParameters(std::string &source, float &hz, std::string &transform) override;
};

class Writer
{
protected:
  int batch_number_;

public:
  //! \brief Write data to file
  // \param [in] d data to write
  virtual void WriteData(const std::vector<std::vector<float>> &d) = 0;

#ifdef MULTI_TH
  //! \brief Multithread write function
  virtual void WriteData() = 0;
#endif

  //! Create graphics
  // \param [in] x data to save to graphic
  // \param [in] out_file name of file for output data
  void CreateGraph(const std::vector<std::vector<float>> &x, std::string out_file);

#ifdef MULTI_TH
  std::mutex *pool_mutex_;
  std::condition_variable *pool_cond_;
  std::queue<std::vector<std::vector<float>>> *data_queue_;
  bool to_continue_;
#endif
};

//! \brief Stands for txt data storing
class WriterTxtData : public Writer
{
private:
  std::ofstream ofs_;
  std::string file_name_;

public:
  WriterTxtData(const std::string &file_name);

  void WriteData(const std::vector<std::vector<float>> &d) override;
#ifdef MULTI_TH
  void WriteData();

#endif
};
#endif