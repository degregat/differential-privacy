//
// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <iostream>
using namespace std;

#include "algorithms/int-scale-gaussian.h"
#include "algorithms/rand.h"
#include "base/testing/status_matchers.h"

namespace differential_privacy {
namespace {

template <typename T>
class IntScaleGaussianTest : public ::testing::Test {};

typedef ::testing::Types<int64_t> NumericTypes;
TYPED_TEST_SUITE(IntScaleGaussianTest, NumericTypes);

TEST(IntScaleGaussianTest, IntScaleGaussianGranularity) {
  // Values from GaussianRoundsToGranularity_Int
  // in numerical-mechanisms_test.cc.

  // med_granularity_mech parameters lead to stddev of 7.7476e+16.
  // large_granularity_mech parameters lead to stddev of 3.96677e+19.
  IntScaleGaussian IntScaleGaussian(1);

  int64_t med_granularity = IntScaleGaussian.ComputeGranularity(7.7476e+16);
  int64_t large_granularity = IntScaleGaussian.ComputeGranularity(3.96677e+19);

  EXPECT_EQ(med_granularity, 2);
  EXPECT_EQ(large_granularity, 1024);
}

TEST(IntScaleGaussianTest, IntScaleGaussianRoundsToGranularity_Int) {
  // Test whether combining ComputeGranularity and SampleIntScaledNoise
  // do the right thing.

  // Implementing this test on the aggregation side would be a good idea,
  // since "input" will be secret shares in deployment.  
  double med_scale = 7.7476e+16;
  double large_scale = 3.96677e+19;

  IntScaleGaussian med_granularity_mech = IntScaleGaussian(med_scale);
  IntScaleGaussian large_granularity_mech = IntScaleGaussian(large_scale);

  for (int i = 0; i < 1e4; ++i) {
    // The rounding process should be independent of the value of x. Setting x
    // to a value between -1*10^6 and 10^6 at random should cover a broad range
    // of congruence classes.
    int64_t input = UniformDouble() * 2e6 - 1e6;

    int64_t med_noise =
      RoundToNearestInt64Multiple(input, med_granularity_mech.ComputeGranularity(1))+
      med_granularity_mech.SampleIntScaledNoise(1);

    int64_t large_noise =
      RoundToNearestInt64Multiple(input, large_granularity_mech.ComputeGranularity(1))+
      large_granularity_mech.SampleIntScaledNoise(1);

    EXPECT_EQ(std::fmod(med_noise, 2), 0);
    EXPECT_EQ(std::fmod(large_noise, 1024), 0);
  }
}

}  // namespace
}  // namespace differential_privacy
