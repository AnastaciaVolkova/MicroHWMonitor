#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath>
#include "read_write.hpp"
#include "tinyxml2.h"
#include <stdexcept>
#include "data_source.hpp"
#include <vector>
#include "transform.hpp"
#include <memory>
#include <future>

using std::invalid_argument;
using std::ofstream;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;
using std::chrono::milliseconds;
using std::chrono::system_clock;

// Function to show number of processed samples
void ShowSamples(int sample_number, bool to_r)
{
  auto cyc = [](char c, int m) {
    for (int i = 0; i < m; i++)
      std::cout << c;
    std::cout.flush();
  };
  string xstr = to_string(sample_number);
  string info = "Number of read samples: ";
  cyc(' ', info.size() + xstr.size() + 1);
  cyc('\r', info.size() + xstr.size() + 1);

  std::cout << info << sample_number;

  if (to_r)
    cyc('\r', info.size() + xstr.size() + 1);
  else
    std::cout << std::endl;
};

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

  string source;
  float hz;
  string transform = "";
  reader->GetParameters(source, hz, transform);
  float tp = 1000 * (1 / hz);
  unique_ptr<DataSource> ds = DataSourceGenerator::GetDataSource(source);

  writer = new WriterTxtData(argv[2], transform != "", hz);

  vector<float> x;

  int sample_number = 0;

  // Run thread to catch user input
  auto key = std::async(std::launch::async, []() {string key; std::cin >> key; return key; });

  // Time, when iteration starts
  auto starts = std::chrono::system_clock::now();

  // Main cycle
  while (true)
  {
    sample_number++;
    float freq = ds->GetValue();
    x.push_back(freq);
    if (x.size() == batch_num)
    {
      if (transform == "fft")
      {
        vector<float> y;
        Transformer::fft(x, y);
        writer->WriteData(y);
      }
      else
      {
        writer->WriteData(x);
      }
      x.clear();
    }
    if ((sample_number % 10) == 0)
    {
      ShowSamples(sample_number, true);
    }
    // Wait rest of time for user keystroke
    system_clock::time_point tw = starts + milliseconds(static_cast<int>(round(tp)));
    std::future_status status = key.wait_until(tw);
    if (status == std::future_status::ready)
    {
      ShowSamples(sample_number, false);
      break;
    }
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - starts).count() <= tp)
      ;
    starts = std::chrono::system_clock::now();
  }
  return 0;
}