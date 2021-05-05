#ifndef DIFFERENTIAL_PRIVACY_ALGORITHMS_NUMERICAL_INT_SCALE_GAUSSIAN_C_H_
#define DIFFERENTIAL_PRIVACY_ALGORITHMS_NUMERICAL_INT_SCALE_GAUSSIAN_C_H_

#include <stdint.h> // to provide int64_t for C

#ifdef __cplusplus
namespace differential_privacy {

extern "C"
{
#endif // __cplusplus

  typedef struct IntScaleGaussian ISG;

  ISG* newIntScaleGaussian(double stddev);

  void deleteIntScaleGaussian(ISG* isg);

  int64_t SampleIntNoise(ISG* isg, double scale);

  int64_t ComputeGranularity(ISG* isg, double scale);

#ifdef __cplusplus
}

} // namespace differential_privacy
#endif // __cplusplus
#endif // DIFFERENTIAL_PRIVACY_ALGORITHMS_NUMERICAL_INT_SCALE_GAUSSIAN_C_H_
