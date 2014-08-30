#include "random_sampler.hpp"

RandomSampler::RandomSampler(unsigned sample_count)
 : Sampler()
 , num_samples_left_(0)
 , prng_(std::time(0))
 , distribution_(0.0, 1.0)
 {
  total_samples_ = sample_count;
 }

Sample
RandomSampler::next_sample()
{

  auto sx = distribution_(prng_);
  auto sy = distribution_(prng_);

  Sample smp = Sample(sx, sy);

  if (++num_samples_left_ >= total_samples_) {
    samples_left_ = false;
  }

  smp.x = smp.x * (max_x_ - min_x_) + min_x_;
  smp.y = smp.y * (max_y_ - min_y_) + min_y_;

  return smp;
}

void
RandomSampler::reset()
{
  samples_left_ = true;
}