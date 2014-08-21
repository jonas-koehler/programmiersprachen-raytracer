#include "sampler.hpp"

Sampler::Sampler()
 : samples_left_(true)
{}

bool
Sampler::samples_left() const
{
  return samples_left_;
}
