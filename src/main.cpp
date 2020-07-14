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
using std::make_unique;
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

bool ParseCommandLine(int argc, char *argv[], string &in_d_file, string &in_file, string &out_file)
{
  int i = 1;
  in_file = "";
  out_file = "";
  in_d_file = "";
  string command_line = argv[0] + string(" -if <input_parameter_file> -of <output_data_file> [-id <input_data_file>]");
  while (i < argc - 1)
  {
    string arg_it = argv[i++];
    if (arg_it == "-if")
    {
      in_file = argv[i++];
    }
    else if (arg_it == "-of")
    {
      out_file = argv[i++];
    }
    else if (arg_it == "-id")
    {
      in_d_file = argv[i++];
    }
    else if (arg_it == "-h")
    {
      std::cout << command_line << std::endl;
      return false;
    }
    else
    {
      std::cerr << "Wrong command line" << std::endl;
      std::cerr << command_line << std::endl;
      return false;
    }
  }
  if ((out_file == "") || (in_file == ""))
  {
    std::cerr << "Wrong command line" << std::endl;
    std::cerr << command_line << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  unique_ptr<Reader> reader;
  unique_ptr<DataSource> ds;
  unique_ptr<Writer> writer;
  unique_ptr<Transformer> transformer;

  // Size of data chunk which is written to file
  constexpr int batch_num = 64;

  // Parametes to get from command line
  string in_file, out_file, in_d_file;

  // Parameters of data source pooling
  string source;
  float hz;
  string transform = "";

  float tp; // Time period to poll sensors

  if (!ParseCommandLine(argc, argv, in_d_file, in_file, out_file))
    return -1;

  try
  {
    reader = make_unique<ReaderXML>(in_file);
  }
  catch (invalid_argument exception)
  {
    std::cerr << exception.what() << std::endl;
    return -1;
  }

  reader->GetParameters(source, hz, transform);
  tp = 1000 * (1 / hz);
  ds = DataSourceGenerator::GetDataSource(in_d_file == "" ? source : in_d_file);

  if (in_d_file == "")
  {
    if (transform == "")
      transformer = make_unique<ByPassTransformer>();
    else
      transformer = make_unique<FFTTransformer>();
  }
  else
  {
    if (transform == "")
      transformer = make_unique<ByPassTransformer>();
    else
      transformer = make_unique<IFFTTransformer>();
  }
  writer = make_unique<WriterTxtData>(out_file);

  DataProcessor data_proc(batch_num, transformer.get(), writer.get());

  // Run thread to catch user input
  auto key = std::async(std::launch::async, []() {string key; std::cin >> key; return key; });

  // Time, when iteration starts
  auto starts = std::chrono::system_clock::now();

  // Main cycle
  while (true)
  {
    float val = ds->GetValue();
    data_proc.Feed(val);

    if ((data_proc.NumSamples() % 10) == 0)
    {
      ShowSamples(data_proc.NumSamples(), true);
    }
    // Wait rest of time for user keystroke
    system_clock::time_point tw = starts + milliseconds(static_cast<int>(round(tp)));
    std::future_status status = key.wait_until(tw);
    if (status == std::future_status::ready)
    {
      ShowSamples(data_proc.NumSamples(), false);
      break;
    }
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - starts).count() <= tp)
      ;
    starts = std::chrono::system_clock::now();
  }
  return 0;
}