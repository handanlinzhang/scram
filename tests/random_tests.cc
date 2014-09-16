#include <gtest/gtest.h>

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <set>

#include "random.h"

using namespace scram;

// Plots the sampled numbers in the range [0, 1).
void PlotDistribution(const std::multiset<double>& series) {
  assert(*series.begin() >= 0);  // Min element.
  assert(*series.rbegin() < 1);  // Max element.
  int num_bins = 50;
  double bin_width = 1.0 / num_bins;
  std::vector<int> bin_hight;
  int size = 0;
  std::multiset<double>::const_iterator it = series.begin();
  for (int bin = 0; bin < num_bins; ++bin) {
    double upper_bound = bin * bin_width;
    int size = 0;
    while (*it < upper_bound) {
      ++size;
      ++it;
      if (it == series.end()) break;
    }
    bin_hight.push_back(size);
  }
  assert(bin_hight.size() == num_bins);
  int max_size = *std::max_element(bin_hight.begin(), bin_hight.end());
  int screen_hight = 20;  // Max number of characters in hight.
  for (int i = 0; i < num_bins; ++i) {
    int num_x = screen_hight * bin_hight[i] / double(max_size) + 0.5;
    bin_hight[i] = num_x;  // Hight in characters.
  }
  std::string hight_char = "x";
  for (int i = screen_hight; i > 0; --i) {
    std::cout << "    ";
    for (int j = 0; j < bin_hight.size(); ++j) {
      (i <= bin_hight[j]) ? std::cout << hight_char : std::cout << " ";
    }
    std::cout << std::endl;
  }
  std::cout << "    0" << std::right << std::setw(num_bins + 1)
      << "1\n" << std::endl;
}

TEST(RandomTest, UniformReal) {
  Random* rng = new Random(std::time(0));
  std::multiset<double> series;
  int sample_size = 1e5;
  for (int i = 0; i < sample_size; ++i) {
    series.insert(rng->UniformRealGenerator(0, 1));
  }
  std::cout << "\n    Uniform Distribution of " << sample_size
      << " Real Numbers.\n" << std::endl;
  PlotDistribution(series);
  delete rng;
}

TEST(RandomTest, Triangular) {
  Random* rng = new Random(std::time(0));
  std::multiset<double> series;
  int sample_size = 1e5;
  for (int i = 0; i < sample_size; ++i) {
    series.insert(rng->TriangularGenerator(0, 0.5, 1));
  }
  std::cout << "\n    Triangular Distribution of " << sample_size
      << " Real Numbers.\n" << std::endl;
  PlotDistribution(series);
  delete rng;
}

TEST(RandomTest, Normal) {
  Random* rng = new Random(std::time(0));
  std::multiset<double> series;
  int sample_size = 1e5;
  for (int i = 0; i < sample_size; ++i) {
    double sample = 0;
    do {
      sample = rng->NormalGenerator(0.5, 0.15);
    } while (sample < 0 || sample >= 1);
    series.insert(sample);
  }
  assert(series.size() == sample_size);
  std::cout << "\n    Normal Distribution of " << sample_size
      << " Real Numbers.\n" << std::endl;
  PlotDistribution(series);
  delete rng;
}

TEST(RandomTest, LogNormal) {
  Random* rng = new Random(std::time(0));
  std::multiset<double> series;
  int sample_size = 1e5;
  for (int i = 0; i < sample_size; ++i) {
    double sample = 0;
    do {
      sample = rng->LogNormalGenerator(0.3, 0.2);
    } while (sample < 0 || sample >= 1);
    series.insert(sample);
  }
  assert(series.size() == sample_size);
  std::cout << "\n    Log-Normal Distribution of " << sample_size
      << " Real Numbers.\n" << std::endl;
  PlotDistribution(series);
  delete rng;
}

TEST(RandomTest, Gamma) {
  Random* rng = new Random(std::time(0));
  std::multiset<double> series;
  int sample_size = 1e5;
  for (int i = 0; i < sample_size; ++i) {
    double sample = 0;
    do {
      sample = rng->GammaGenerator(2, 2) / 10;
    } while (sample < 0 || sample >= 1);
    series.insert(sample);
  }
  assert(series.size() == sample_size);
  std::cout << "\n    Gamma Distribution of " << sample_size
      << " Real Numbers.\n" << std::endl;
  PlotDistribution(series);
  delete rng;
}

TEST(RandomTest, Beta) {
  Random* rng = new Random(std::time(0));
  std::multiset<double> series;
  int sample_size = 1e5;
  for (int i = 0; i < sample_size; ++i) {
    double sample = 0;
    do {
      sample = rng->BetaGenerator(2, 2);
    } while (sample < 0 || sample >= 1);
    series.insert(sample);
  }
  assert(series.size() == sample_size);
  std::cout << "\n    Beta Distribution of " << sample_size
      << " Real Numbers.\n" << std::endl;
  PlotDistribution(series);
  delete rng;
}

TEST(RandomTest, Poisson) {
  Random* rng = new Random(std::time(0));
  std::multiset<double> series;
  int sample_size = 1e5;
  for (int i = 0; i < sample_size; ++i) {
    double sample = 0;
    do {
      sample = rng->PoissonGenerator(5) / 10;
    } while (sample < 0 || sample >= 1);
    series.insert(sample);
  }
  assert(series.size() == sample_size);
  std::cout << "\n    Poisson Distribution of " << sample_size
      << " Real Numbers.\n" << std::endl;
  PlotDistribution(series);
  delete rng;
}
