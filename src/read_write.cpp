#include "read_write.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>
#include "matplotlibcpp.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace plt = matplotlibcpp;
using std::invalid_argument;
using std::ostringstream;
using std::string;
using std::to_string;
using std::tuple;
using std::vector;

#ifdef MULTI_TH
using std::move;
using std::mutex;
using std::queue;
using std::unique_lock;
#endif

ReaderXML::ReaderXML(const string &in_file)
{
  tinyxml2::XMLError error = doc_.LoadFile(in_file.c_str());
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

WriterTxtData::WriterTxtData(const string &file_name) : file_name_(file_name)
{
  Writer::batch_number_ = 0;
  ofs_.open(file_name_);
}

void WriterTxtData::WriteData(const vector<vector<float>> &d)
{
  for (int i = 0; i < d[0].size(); i++)
  {
    for (int j = 0; j < d.size(); j++)
      ofs_ << std::fixed << d[j][i] << " ";
  }
  ofs_ << std::endl;
  CreateGraph(d, file_name_);
  batch_number_++;
}

#ifdef MULTI_TH
void WriterTxtData::WriteData()
{
  while (to_continue_)
  {
    queue<vector<vector<float>>> working_queue;
    {
      unique_lock<mutex> lock(*pool_mutex_);
      pool_cond_->wait(lock, [this]() { return !data_queue_->empty(); });
      while (!data_queue_->empty())
      {
        working_queue.push(move(data_queue_->front()));
        data_queue_->pop();
      }
    }
    while (!working_queue.empty())
    {
      vector<vector<float>> q = move(working_queue.front());
      WriteData(q);
      working_queue.pop();
    }
  }
}
#endif

void Writer::CreateGraph(const vector<vector<float>> &x, string out_file)
{
  int dot_pos = out_file.find_last_of('.');
  if (dot_pos != string::npos)
    out_file.erase(dot_pos);

  ostringstream oss;
  oss << std::setw(4) << std::setfill('0') << to_string(batch_number_);
  string out_file_png = out_file + "_" + oss.str() + ".png";
  int series_num = x.size();
  for (int sn = 0; sn < series_num; sn++)
  {
    plt::subplot(series_num, 1, sn + 1);
    plt::plot(x[sn]);
  }
  plt::save(out_file_png);
  plt::clf();
};