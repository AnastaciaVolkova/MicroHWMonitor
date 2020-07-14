#ifndef SENSOR_POLLER_
#define SENSOR_POLLER_

#include "sensor_poller.hpp"
#include "transform.hpp"
#include "data_source.hpp"
#include "read_write.hpp"
#include "tinyxml2.h"
#include <thread>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <memory>
#include <future>
#include <iostream>

//!\brief Sensor polling
class Poller
{
public:
  Poller(std::string param_file_name, std::string output_file_name, std::string input_data_file_name, int batch_num = 64) : batch_num_(batch_num)
  {

    // Parameters of data source pooling
    std::string source;
    float hz;
    std::string transform = "";

    // Read parameters
    reader_ = std::make_unique<ReaderXML>(param_file_name);
    reader_->GetParameters(source, hz, transform);

    period_ = 1000 * (1 / hz);
    data_source_ = DataSourceGenerator::GetDataSource(input_data_file_name == "" ? source : input_data_file_name);

    writer_ = std::make_unique<WriterTxtData>(output_file_name);
    if (input_data_file_name == "")
    {
      if (transform == "")
        transformer = std::make_unique<ByPassTransformer>();
      else

        transformer = std::make_unique<FFTTransformer>();
      data_proc_ = std::make_unique<DataProcessor>(batch_num_, transformer.get(), writer_.get());
    }
    else
    {
      if (transform == "")
        transformer = std::make_unique<ByPassTransformer>();
      else
        transformer = std::make_unique<IFFTTransformer>();
      data_proc_ = std::make_unique<DataProcessor>(2 * batch_num_, transformer.get(), writer_.get());
      period_ /= 2;
    }
  };
  void PollSensors()
  {
    // Run thread to catch user input
    auto key = std::async(std::launch::async, []() {std::string key; std::cin >> key; return key; });

    // Time, when iteration starts
    auto starts = std::chrono::system_clock::now();

    // Main cycle
    while (true)
    {
      float val = data_source_->GetValue();
      data_proc_->Feed(val);
      if (data_source_->NoData())
        break;

      if ((data_proc_->NumSamples() % 10) == 0)
      {
        ShowSamples(data_proc_->NumSamples(), true);
      }
      // Wait rest of time for user keystroke
      std::chrono::system_clock::time_point tw = starts + std::chrono::milliseconds(static_cast<int>(round(period_)));
      std::future_status status = key.wait_until(tw);
      if (status == std::future_status::ready)
      {
        ShowSamples(data_proc_->NumSamples(), false);
        break;
      }
      while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - starts).count() <= period_)
        ;
      starts = std::chrono::system_clock::now();
    }
  };

private:
  // Size of data chunk which is written to file
  const int batch_num_ = 64;

  // Function to show number of processed samples
  void ShowSamples(int sample_number, bool to_r)
  {
    auto cyc = [](char c, int m) {
      for (int i = 0; i < m; i++)
        std::cout << c;
      std::cout.flush();
    };
    std::string xstr = std::to_string(sample_number);
    std::string info = "Number of read samples: ";
    cyc(' ', info.size() + xstr.size() + 1);
    cyc('\r', info.size() + xstr.size() + 1);

    std::cout << info << sample_number;

    if (to_r)
      cyc('\r', info.size() + xstr.size() + 1);
    else
      std::cout << std::endl;
  };

private:
  std::unique_ptr<Reader> reader_;
  std::unique_ptr<DataSource> data_source_;
  std::unique_ptr<Writer> writer_;
  std::unique_ptr<Transformer> transformer;
  std::unique_ptr<DataProcessor> data_proc_;
  float period_;
};

#endif