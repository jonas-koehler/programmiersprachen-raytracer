#ifndef BUW_STANDARD_GRID_SAMPLER
#define BUW_STANDARD_GRID_SAMPLER

class StandardGridSampler : public Sampler
{
public:
  StandardGridSampler(unsigned res_x, unsigned res_y);
  Sample next_sample() override;
private:
  unsigned current_x_, curent_y_;
};

#endif // BUW_STANDARD_GRID_SAMPLER
