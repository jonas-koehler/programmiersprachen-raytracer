#include "sampler.hpp"

Sampler::Sampler()
 : samples_left_(true)
 , total_samples_(0)
 , min_x_(0)
 , min_y_(0)
 , max_x_(1)
 , max_y_(1)
{}

void
Sampler::restrict(float min_x, float min_y, float max_x, float max_y)
{
  min_x_ = min_x;
  min_y_ = min_y;
  max_x_ = max_x;
  max_y_ = max_y;
}

bool
Sampler::samples_left() const
{
  return samples_left_;
}

unsigned
Sampler::total_samples() const
{
  return total_samples_;
}
