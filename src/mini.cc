#include "mini/mini.h"

#include <deque>
#include <iostream>

namespace mini {

namespace detail {

/* clang-format off */
constexpr static std::uint8_t kNucleotideCoder[] = {
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255,   0, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255,   0,   1 ,  1,   0, 255, 255,   2,
      3, 255, 255,   2, 255,   1,   0, 255,
    255, 255,   0,   1,   3,   3,   2,   0,
    255,   3, 255, 255, 255, 255, 255, 255,
    255,   0,   1,   1,   0, 255, 255,   2,
      3, 255, 255,   2, 255,   1,   0, 255,
    255, 255,   0,   1,   3,   3,   2,   0,
    255,   3, 255, 255, 255, 255, 255, 255
};
/* clang-format on */

}  // namespace detail

auto Hash(std::uint64_t val, std::uint64_t const kMask) -> std::uint64_t {
  val = ((~val) + (val << 21)) & kMask;
  val = val ^ (val >> 24);
  val = ((val + (val << 3)) + (val << 8)) & kMask;
  val = val ^ (val >> 14);
  val = ((val + (val << 2)) + (val << 4)) & kMask;
  val = val ^ (val >> 28);
  val = (val + (val << 31)) & kMask;
  return val;
}

auto Minimize(std::string const& seq, std::uint8_t const kmer_len,
              std::uint8_t const win_len) -> std::vector<KMer> {
  auto dst = std::vector<KMer>();
  auto const kMask = (1ULL << (kmer_len * 2ULL)) - 1ULL;

  auto const hash = [kMask](std::uint64_t val) -> std::uint64_t {
    return Hash(val, kMask);
  };

  auto curr_kmer_val = std::uint64_t(0);
  auto rc_curr_kmer_val = std::uint64_t(0);
  auto window = std::deque<std::pair<std::uint64_t, KMer>>();

  auto const shift_kmer = [kMask](std::uint64_t const kmer,
                                  char const base) -> std::uint64_t {
    return ((kmer << 2ULL) |
            detail::kNucleotideCoder[static_cast<std::size_t>(base)]) &
           kMask;
  };

  auto const shift_rc_kmer = [kmer_len](std::uint64_t const kmer,
                                        char const base) -> std::uint64_t {
    return (kmer >> 2ULL) |
           ((3ULL ^ detail::kNucleotideCoder[static_cast<std::size_t>(base)])
            << ((kmer_len - 1ULL) * 2ULL));
  };

  auto const window_push = [&window](std::uint64_t const hash,
                                     KMer const kmer) -> void {
    while (!window.empty() && window.back().first > hash) {
      window.pop_back();
    }

    window.emplace_back(hash, kmer);
  };

  auto const update_window = [&window, &dst](std::uint32_t const pos) -> void {
    while (!window.empty() && (window.front().second.position() <= pos)) {
      window.pop_front();
    }
  };

  for (auto i = 0U; i < seq.size(); ++i) {
    curr_kmer_val = shift_kmer(curr_kmer_val, seq[i]);
    rc_curr_kmer_val = shift_rc_kmer(rc_curr_kmer_val, seq[i]);
    if (i + 1U >= kmer_len) {
      if (curr_kmer_val < rc_curr_kmer_val) {
        window_push(hash(curr_kmer_val),
                    KMer(curr_kmer_val, i + 1U - kmer_len, 0));
      } else if (curr_kmer_val > rc_curr_kmer_val) {
        window_push(hash(rc_curr_kmer_val),
                    KMer(rc_curr_kmer_val, i + 1U - kmer_len, 1));
      }
    }
    if (i >= kmer_len - 1U + win_len - 1U) {
      if (dst.empty() ||
          dst.back().position() != window.front().second.position()) {
        dst.push_back(window.front().second);
      }
      update_window(i - (kmer_len - 1U + win_len - 1U));
    }
  }

  return dst;
}

}  // namespace mini
