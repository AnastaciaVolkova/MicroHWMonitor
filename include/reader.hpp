#ifndef READER_HPP_
#define READER_HPP_

#include <string>
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

class ReaderXML : public Reader
{
private:
  tinyxml2::XMLDocument doc_;

public:
  ReaderXML(const char *in_file);
  void GetParameters(std::string &source, float &hz, std::string &transform) override;
};

#endif