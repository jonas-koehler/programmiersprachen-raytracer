#ifndef BUW_STANDARD_GRID_SAMPLER
#define BUW_STANDARD_GRID_SAMPLER

#include "sampler.hpp"

class StandardGridSampler : public Sampler
{
public:
  StandardGridSampler(unsigned res_x, unsigned res_y);
  Sample next_sample() override;
private:
  unsigned current_x_, current_y_, res_x_, res_y_;
  float res_x_inv_, res_y_inv_;
};

#endif // BUW_STANDARD_GRID_SAMPLER
