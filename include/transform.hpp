#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_
#include <vector>
#include "read_write.hpp"
#ifdef MULTI_TH
#include <mutex>
#include <condition_variable>
#include <queue>
#endif
class Transformer
{
public:
  //! Performs transformation
  /*!
  \param [in] in_data input data
  \param [out] out_data transformation of input data
  */
  virtual void operator()(std::vector<float> &in_data, std::vector<std::vector<float>> &out_data) = 0;
};

class ByPassTransformer : public Transformer
{
  void operator()(std::vector<float> &in_data, std::vector<std::vector<float>> &out_data) override;
};

class FFTTransformer : public Transformer
{
  void operator()(std::vector<float> &in_data, std::vector<std::vector<float>> &out_data) override;
};

class IFFTTransformer : public Transformer
{
  void operator()(std::vector<float> &in_data, std::vector<std::vector<float>> &out_data) override;
};

//!\brief Stands for data processing
class DataProcessor
{
public:
  DataProcessor(int batch_size, Transformer *transformer, Writer *writer);
  void Feed(float sample);
  int NumSamples() { return num_samples_processed_; };
#ifdef MULTI_TH
  std::mutex *pool_mutex_;
  std::condition_variable *pool_cond_;
  std::queue<std::vector<std::vector<float>>> *data_queue_;
#endif

private:
  std::vector<float> data_;
  int batch_size_;
  Writer *writer_;
  Transformer *transformer_;
  int num_samples_processed_;
};
#endif