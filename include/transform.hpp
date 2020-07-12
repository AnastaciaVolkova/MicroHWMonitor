#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_
#include <vector>
class Transformer
{
public:
  //! Performs fft
  /*!
  \param [in] in_data
  \param [out] out_re - real part of fft 
  \param [out] out_im - image part of fft
  */
  static void fft(const std::vector<float> &in_data, std::vector<float> &out_re, std::vector<float> &out_im);
};
#endif