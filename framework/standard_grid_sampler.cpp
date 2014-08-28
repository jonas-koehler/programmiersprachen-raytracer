#include "standard_grid_sampler.hpp"

StandardGridSampler::StandardGridSampler(unsigned res_x, unsigned res_y)
 : Sampler()
 , current_x_(0)
 , current_y_(0)
 , res_x_(res_x)
 , res_y_(res_y)
 , res_x_inv_(1.0f / ((float) res_x))
 , res_y_inv_(1.0f / ((float) res_y))
 {
   total_samples_ = res_x * res_y;
 }

Sample
StandardGridSampler::next_sample()
{
  float sx = ((float) current_x_) * res_x_inv_;
  float sy = ((float) current_y_) * res_y_inv_;

  Sample smp = Sample(sx, sy);

  if (++current_x_ >= res_x_) {
    current_x_ = 0;
    if (++current_y_ >= res_y_) {
      current_y_ = 0;
      samples_left_ = false;
    }
  }

  smp.x = smp.x * (max_x_ - min_x_) + min_x_;
  smp.y = smp.y * (max_y_ - min_y_) + min_y_;

  return smp;
}

void
StandardGridSampler::reset()
{
  samples_left_ = true;
}