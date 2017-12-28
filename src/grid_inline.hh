#ifndef SRC_GRID_INLINE_HH_
#define SRC_GRID_INLINE_HH_

namespace mgrid {

template <typename F>
void Grid::points_iter(F f) {
  const std::size_t w = res_.x;
  const std::size_t h = res_.y;

  for (std::size_t y = 0; y < h; y++) {
    for (std::size_t x = 0; x < w; x++) {
      f(points_[y * w + x]);
    }
  }
}

template <typename F>
void Grid::triangle_iter(F f) {
  const std::size_t w = res_.x;
  const std::size_t h = res_.y;

  for (std::size_t y = 1; y < h; y++) {
    for (std::size_t x = 1; x < w; x++) {
      const std::size_t ind[4] = {
          // clang-format off
          (y - 1) * w + (x - 1),
          (y - 1) * w + (x - 0),
          (y - 0) * w + (x - 1),
          (y - 0) * w + (x - 0),
          // clang-format on
      };
      const std::size_t pattern[2][3] = {
          // clang-format off
          {0, 2, 1},
          {0, 3, 2}
          // clang-format on
      };
      for (int t = 0; t < 2; t++) {
        f(std::array<std::size_t, 3>{ind[pattern[t][0]], ind[pattern[t][1]],
                                     ind[pattern[t][2]]});
      }
    }
  }
}

}  // mgrid

#endif  // SRC_GRID_INLINE_HH_
