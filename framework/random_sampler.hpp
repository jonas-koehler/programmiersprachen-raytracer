#ifndef BUW_RANDOM_SAMPLER
#define BUW_RANDOM_SAMPLER

#include "sampler.hpp"

#include <random>
#include <ctime>

class RandomSampler : public Sampler
{
public:
  RandomSampler(unsigned sample_count);
  Sample next_sample() override;
  void reset() override;
private:
  unsigned num_samples_left_;
  std::default_random_engine prng_;
  std::uniform_real_distribution<double> distribution_;
};

#endif // BUW_RANDOM_SAMPLER
