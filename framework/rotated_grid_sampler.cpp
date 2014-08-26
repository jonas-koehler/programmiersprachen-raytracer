#include "rotated_grid_sampler.hpp"

RotatedGridSampler::RotatedGridSampler(unsigned res_x, unsigned res_y, float alpha_deg_)
 : Sampler()
 , current_x_(0)
 , current_y_(0)
 , res_x_(res_x/2)
 , res_y_(res_y/2)
 , res_x_inv_(2 / ((float) res_x))
 , res_y_inv_(2 / ((float) res_y))
 , grid_pos_(0)
 , sin_alpha_2(std::sin(alpha_deg_ * M_PI / 180.0f) * 0.5f)
 , cos_alpha_2(std::cos(alpha_deg_ * M_PI / 180.0f) * 0.5f)
 {
   total_samples_ = res_x * res_y;
 }

Sample
RotatedGridSampler::next_sample()
{
  float sx = current_x_ + 0.5f;
  float sy = current_y_ + 0.5f;

  if (grid_pos_ == 0) {
    sx += -cos_alpha_2 ;
    sy += -sin_alpha_2;
  } else if (grid_pos_ == 1) {
    sx += sin_alpha_2;
    sy += -cos_alpha_2;
  } else if (grid_pos_ == 2) {
    sx += cos_alpha_2 ;
    sy += sin_alpha_2;
  } else if (grid_pos_ == 3) {
    sx += -sin_alpha_2;
    sy += cos_alpha_2;
  }

  sx *= res_x_inv_ ;
  sy *= res_y_inv_;

  Sample smp = Sample(sx, sy);

  if (++grid_pos_ >= 3) {
    grid_pos_ = 0;
    if (++current_x_ >= res_x_) {
      current_x_ = 0;
      if (++current_y_ >= res_y_) {
        current_y_ = 0;
        samples_left_ = false;
      }
    }
  }

  smp.x = smp.x * (max_x_ - min_x_) + min_x_;
  smp.y = smp.y * (max_y_ - min_y_) + min_y_;

  return smp;
}
