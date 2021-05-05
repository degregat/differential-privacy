#ifndef DIFFERENTIAL_PRIVACY_ALGORITHMS_NUMERICAL_INT_SCALE_GAUSSIAN_H_
#define DIFFERENTIAL_PRIVACY_ALGORITHMS_NUMERICAL_INT_SCALE_GAUSSIAN_H_

#include <limits>
#include <memory>

#include "algorithms/distributions.h"
#include "algorithms/util.h"

// A class that exposes a lower level interface to the discrete
// gaussian, taking only stddev or scale, for use in iterated learning
// techniques like DPSGD (https://arxiv.org/abs/1607.00133). The
// interface is geared towards the use with multi party secure
// aggregation protocols.

namespace differential_privacy {

class IntScaleGaussian {
public:
  // Since IntScaleGaussian is small and specific class we use a
  // constructor instead of a Builder.
  explicit IntScaleGaussian(double stddev);
  
  // Sample scaled Int64 noise to hand to aggregation side, which adds
  // it to secret shares. Scale is used as an argument to accomodate
  // adding the noise to secret shares of fixed point encodings of
  // input data.
  int64_t SampleIntScaledNoise(double scale);

  // Compute granularity to hand to aggregation side, which does the
  // rounding.
  int64_t ComputeGranularity(double scale);

private: 
  std::unique_ptr<internal::GaussianDistribution> distro_;
};

} // namespace differential_privacy

#endif // DIFFERENTIAL_PRIVACY_ALGORITHMS_NUMERICAL_INT_SCALE_GAUSSIAN_H_
