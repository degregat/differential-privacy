#include "algorithms/int-scale-gaussian-c.h"
#include "algorithms/int-scale-gaussian.h"

namespace differential_privacy {

extern "C"
{
  ISG* newIntScaleGaussian(double stddev) {
    return new IntScaleGaussian(stddev);
  }

  void deleteIntScaleGaussian(ISG* isg) {
    delete isg;
  }

  int64_t SampleIntNoise(ISG* isg, double scale) {
    return isg->SampleIntScaledNoise(scale);
  }

  int64_t ComputeGranularity(ISG* isg, double scale) {
    return isg->ComputeGranularity(scale);
  }
}

} // namespace differential_privacy
