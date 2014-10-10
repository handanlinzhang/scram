#include <gtest/gtest.h>

#include "performance_tests.h"

using namespace scram;

// Performance testting is done only if requested by activating
// disabled tests.
//
// To run the performance tests, supply "--gtest_also_run_disable_tests" flag
// to GTest. The GTest filter may applied to sortout only performance tests.
// Different tests are compiled depending on the build type. Generally,
// debug or non-debug types are recognized.
//
// Performance testing values are taken from a computer with the following
// specs:
//   Core i5-2410M, Ubuntu 14.04 64bit on VirtualBox
//   Compilation with GCC 4.8.2 and Boost 1.55
//
// The values for performance are expected to have some random variation.
// Better as well as worse performance are reported as test failures to
// indicate the change.
//
// NOTE: Running all the tests may take considerable time.
// NOTE: Running tests several times is recommended to take into account
//       the variation of time results.

// Tests the performance of probability calculations.
// Both with and without approximations tests are done.
TEST_F(PerformanceTest, DISABLED_ThreeMotor) {
  double p_time_std = 0.650;
  double p_time_full = 30.000;  // Without cut-off approximation.
#ifdef NDEBUG
  p_time_std = 0.130;
  p_time_full = 6.000;
#endif
  std::string input = "./share/scram/input/benchmark/three_motor.xml";
  ASSERT_NO_THROW(ran->ProcessInput(input));
  ASSERT_NO_THROW(ran->Analyze());  // Standard analysis.
  double delta_sqr = std::abs(p_total() - 0.0211538);
  EXPECT_TRUE(delta_sqr < 1e-5);
  EXPECT_GT(ProbCalcTime(), p_time_std * (1 - delta));
  EXPECT_LT(ProbCalcTime(), p_time_std * (1 + delta));

  delete ran;
  ran = new RiskAnalysis();
  settings.cut_off(0);  // No approximation.
  ran->AddSettings(settings);
  ASSERT_NO_THROW(ran->ProcessInput(input));
  ASSERT_NO_THROW(ran->Analyze());
  delta_sqr = std::abs(p_total() - 0.0211538);
  EXPECT_TRUE(delta_sqr < 1e-5);
  EXPECT_GT(ProbCalcTime(), p_time_full * (1 - delta));
  EXPECT_LT(ProbCalcTime(), p_time_full * (1 + delta));
}
