#include "algorithms/int-scale-gaussian.h"

namespace differential_privacy {
  
IntScaleGaussian::IntScaleGaussian(double stddev) {
  base::StatusOr<std::unique_ptr<internal::GaussianDistribution>>
    status_or_distro =
    internal::GaussianDistribution::Builder().SetStddev(stddev).Build();
  DCHECK(status_or_distro.status().ok());
  distro_ = std::move(status_or_distro.value());
}

int64_t IntScaleGaussian::SampleIntScaledNoise(double scale) {
  double sample = distro_->Sample(scale);
  
  SafeOpResult<int64_t> noise_cast_result =
    SafeCastFromDouble<int64_t>(std::round(sample));
  
  return noise_cast_result.value;
}

int64_t IntScaleGaussian::ComputeGranularity(double scale) {
  // Granularity should be a power of 2, and thus can be cast without losing
  // any meaningful fraction. If granularity is <1 (i.e., 2^x, where x<0),
  // then flooring the granularity we use here to 1 should be fine for this
  // function. If granularity is greater than an int64_t can represent, then
  // it's so high that the return value likely won't be terribly meaningful,
  // so just cap the granularity at the largest number int64_t can represent.
  int64_t granularity;
  SafeOpResult<int64_t> granularity_cast_result = SafeCastFromDouble<int64_t>(
      std::max(distro_->GetGranularity(scale), 1.0));
  if (granularity_cast_result.overflow) {
    granularity = std::numeric_limits<int64_t>::max();
  } else {
    granularity = granularity_cast_result.value;
  }

  return granularity;
}
  
} // namespace differential_privacy
