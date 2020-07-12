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

using std::ifstream;
using std::invalid_argument;
using std::string;
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
  auto starts = std::chrono::system_clock::now();
  DataSource *ds = new CpuFreqData();

  vector<float> x, y_re(64), y_im(64);

  auto show = [](const auto &v) {
    for (auto i : v)
      std::cout << i << " ";
    std::cout << std::endl;
  };

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
    }
  }
  return 0;
}
