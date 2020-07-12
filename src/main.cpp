#include <string>
#include <iostream>
#include <chrono>
#include <fstream>

#include "reader.hpp"
#include "tinyxml2.h"
#include <stdexcept>
#include "data_source.hpp"
#include <vector>
#include "transform.hpp"
#include <memory>

using std::ifstream;
using std::invalid_argument;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

int main(int argc, char *argv[])
{
  Reader *reader;
  try
  {
    reader = new ReaderXML(argv[1]);
  }
  catch (invalid_argument exception)
  {
    std::cerr << exception.what() << std::endl;
    return -1;
  }
  string source;
  float hz;
  string transform = "";
  reader->GetParameters(source, hz, transform);
  float tp = 1000 * (1 / hz);
  unique_ptr<DataSource> ds = DataSourceGenerator::GetDataSource(source);

  vector<float> x, y_re(64), y_im(64);

  auto starts = std::chrono::system_clock::now();
  while (true)
  {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - starts).count() >= tp)
    {
      starts = std::chrono::system_clock::now();
      float freq = ds->GetValue();
      x.push_back(freq);
      if (transform == "fft")
      {
        if (x.size() == 64)
        {
          Transformer::fft(x, y_re, y_im);
          x.clear();
        }
      }
      if ((x.size() % 10) == 0)
      {
        string xstr = to_string(x.size());
        string info = "Number of read samples: ";
        std::cout << info << x.size();
        for (int i = 0; i < info.size() + xstr.size(); i++)
          std::cout << "\r";
        std::cout.flush();
      }
    }
  }
  return 0;
}
