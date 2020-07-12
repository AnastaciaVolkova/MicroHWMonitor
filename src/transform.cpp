#include "transform.hpp"
#include <fftw3.h>

using std::vector;

void Transformer::fft(const std::vector<float> &in_data, std::vector<float> &out_re, std::vector<float> &out_im)
{
  fftw_complex *in, *out;
  in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 64);
  out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * 64);
  fftw_plan p = fftw_plan_dft_1d(64, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  for (int i = 0; i < 64; i++)
  {
    in[i][0] = in_data[i];
    in[i][1] = 0;
  }
  fftw_execute(p);

  for (int i = 0; i < 64; i++)
  {
    out_re[i] = out[i][0];
    out_im[i] = out[i][1];
  }

  fftw_destroy_plan(p);
  fftw_free(in);
  fftw_free(out);
};