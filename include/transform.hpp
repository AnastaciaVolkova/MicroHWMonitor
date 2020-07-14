#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_
#include <vector>
#include "read_write.hpp"

class Transformer
{
public:
  //! Performs transformation
  /*!
  \param [in] in_data input data
  \param [out] out_data transformation of input data
  */
  virtual void operator()(std::vector<float> &in_data, std::vector<float> &out_data) = 0;
};

class ByPassTransformer : public Transformer
{
  void operator()(std::vector<float> &in_data, std::vector<float> &out_data) override;
};

class FFTTransformer : public Transformer
{
  void operator()(std::vector<float> &in_data, std::vector<float> &out_data) override;
};

class IFFTTransformer : public Transformer
{
  void operator()(std::vector<float> &in_data, std::vector<float> &out_data) override;
};

//!\brief Stands for data processing
class DataProcessor
{
public:
  DataProcessor(int batch_size, Transformer *transformer, Writer *writer);
  void Feed(float sample);
  int NumSamples() { return num_samples_processed_; };

private:
  std::vector<float> data_;
  int batch_size_;
  Writer *writer_;
  Transformer *transformer_;
  int num_samples_processed_;
};
#endif