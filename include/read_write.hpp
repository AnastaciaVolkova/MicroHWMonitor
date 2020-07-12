#ifndef READER_HPP_
#define READER_HPP_

#include <string>
#include <vector>
#include <fstream>
#include "tinyxml2.h"

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
  ReaderXML(const char *in_file);
  void GetParameters(std::string &source, float &hz, std::string &transform) override;
};

class Writer
{
public:
  //! Write data to file
  // \param [in] d data to write
  virtual void WriteData(const std::vector<float> d) = 0;
};

//! \brief Stands for txt data storing
class WriterTxtData : public Writer
{
private:
  std::ofstream ofs_;

public:
  // \param [in] file_name name of file, where data are going to be stored
  // \param [in] is_complex flag, which signals that data to write is complex
  WriterTxtData(std::string file_name, bool is_complex);
  void WriteData(const std::vector<float> d) override;
};
#endif