#include "transform.hpp"
#include <fftw3.h>

using std::vector;

void FFTTransformer::operator()(vector<float> &in_data, vector<float> &out_data)
{
  fftw_complex *in, *out;
  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * in_data.size());
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * in_data.size());
  out_data.resize(2 * in_data.size());
  fftw_plan p = fftw_plan_dft_1d(64, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  for (int i = 0; i < 64; i++)
  {
    in[i][0] = in_data[i];
    in[i][1] = 0;
  }
  fftw_execute(p);
  for (int i = 0; i < in_data.size(); i++)
  {
    out_data[2 * i] = out[i][0];
    out_data[2 * i + 1] = out[i][1];
  }

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);
};

void ByPassTransformer::operator()(vector<float> &in_data, vector<float> &out_data)
{
  out_data = std::move(in_data);
};

DataProcessor::DataProcessor(int batch_size, Transformer *transformer, Writer *writer) : batch_size_(batch_size),
                                                                                         writer_(writer),
                                                                                         transformer_(transformer),
                                                                                         num_samples_processed_(0){};

void DataProcessor::Feed(float sample)
{
  data_.push_back(sample);
  num_samples_processed_++;
  if (data_.size() == batch_size_)
  {
    vector<float> out_data;
    (*transformer_)(data_, out_data);
    writer_->WriteData(out_data);
    data_.clear();
  }
};
