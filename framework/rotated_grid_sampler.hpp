#ifndef BUW_ROTATED_GRID_SAMPLER
#define BUW_ROTATED_GRID_SAMPLER

#include "sampler.hpp"

#include <cmath>

class RotatedGridSampler : public Sampler
{
public:
  RotatedGridSampler(unsigned res_x, unsigned res_y, float alpha_deg_);
  Sample next_sample() override;
  void reset() override;
private:
  unsigned grid_pos_;
  float current_x_, current_y_, res_x_, res_y_;
  float res_x_inv_, res_y_inv_, sin_alpha_2, cos_alpha_2;
};

#endif // BUW_ROTATED_GRID_SAMPLER
