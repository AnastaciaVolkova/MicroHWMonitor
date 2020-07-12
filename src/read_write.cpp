#include "read_write.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

using std::invalid_argument;
using std::string;
using std::tuple;
using std::vector;

ReaderXML::ReaderXML(const char *in_file)
{
  tinyxml2::XMLError error = doc_.LoadFile(in_file);
  switch (error)
  {
  case tinyxml2::XML_SUCCESS:
    break;
  case tinyxml2::XML_NO_ATTRIBUTE:
    throw invalid_argument("Xml file has no attributes");
  case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
    throw invalid_argument("Xml file has wrong attribute type");
  case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
    throw invalid_argument("File not found");
  case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
    throw invalid_argument("File could not be opened");
  case tinyxml2::XML_ERROR_FILE_READ_ERROR:
    throw invalid_argument("File war read with error");
  case tinyxml2::XML_ERROR_PARSING_ELEMENT:
    throw invalid_argument("Parsing element error");
  case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
    throw invalid_argument("Parsing attribute error");
  case tinyxml2::XML_ERROR_PARSING_TEXT:
    throw invalid_argument("Parsing text error");
  case tinyxml2::XML_ERROR_PARSING_CDATA:
    throw invalid_argument("Parsing cdata error");
  case tinyxml2::XML_ERROR_PARSING_COMMENT:
    throw invalid_argument("Parsing comment error");
  case tinyxml2::XML_ERROR_PARSING_DECLARATION:
    throw invalid_argument("Parsing declaration error");
  case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
    throw invalid_argument("Unknown parsing error");
  case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
    throw invalid_argument("XML document is empty");
  case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
    throw invalid_argument("XML element is mismatched");
  case tinyxml2::XML_ERROR_PARSING:
    throw invalid_argument("XML error parsing");
  case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
    throw invalid_argument("Text format error");
  case tinyxml2::XML_NO_TEXT_NODE:
    throw invalid_argument("No text error");
  case tinyxml2::XML_ELEMENT_DEPTH_EXCEEDED:
    throw invalid_argument("Xml depth error");
  default:
    throw invalid_argument("Unkown error");
  }
}

void ReaderXML::GetParameters(string &source, float &hz, string &transform)
{
  source = doc_.RootElement()->FirstChildElement("source")->GetText();
  hz = std::stof(doc_.RootElement()->FirstChildElement("hz")->GetText());
  if (doc_.RootElement()->FirstChildElement("transform") != nullptr)
    transform = doc_.RootElement()->FirstChildElement("transform")->GetText();
}

WriterTxtData::WriterTxtData(string file_name)
{
  ofs_.open(file_name);
}

void WriterTxtData::WriteData(const vector<float> d)
{
  for (auto i : d)
    ofs_ << i << " ";
  ofs_ << std::endl;
}