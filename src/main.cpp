#include <string>
#include <iostream>
#include <chrono>
#include <fstream>

#include "read_write.hpp"
#include "tinyxml2.h"
#include <stdexcept>
#include "data_source.hpp"
#include <vector>
#include "transform.hpp"
#include <memory>

using std::invalid_argument;
using std::ofstream;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

int main(int argc, char *argv[])
{
  Reader *reader;
  Writer *writer;

  // Size of data chunk which is written to file
  constexpr int batch_num = 64;

  try
  {
    reader = new ReaderXML(argv[1]);
  }
  catch (invalid_argument exception)
  {
    std::cerr << exception.what() << std::endl;
    return -1;
  }

  writer = new WriterTxtData(argv[2]);

  string source;
  float hz;
  string transform = "";
  reader->GetParameters(source, hz, transform);
  float tp = 1000 * (1 / hz);
  unique_ptr<DataSource> ds = DataSourceGenerator::GetDataSource(source);

  vector<float> x, y_re(batch_num), y_im(batch_num);

  auto starts = std::chrono::system_clock::now();

  int sample_number = 0;
  while (true)
  {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - starts).count() >= tp)
    {
      starts = std::chrono::system_clock::now();
      sample_number++;
      float freq = ds->GetValue();
      x.push_back(freq);
      if (x.size() == batch_num)
      {
        if (transform == "fft")
          Transformer::fft(x, y_re, y_im);
        else
        {
          writer->WriteData(x);
        }
        x.clear();
      }
      if ((sample_number % 10) == 0)
      {
        string xstr = to_string(sample_number);
        string info = "Number of read samples: ";
        std::cout << info << sample_number;
        for (int i = 0; i < info.size() + xstr.size(); i++)
          std::cout << "\r";
        std::cout.flush();
      }
    }
  }
  return 0;
}
