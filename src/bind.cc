#include <algorithm>
#include <deque>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

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

constexpr static char kNucleotideDecoder[] = {
    'A', 'C', 'G', 'T'
};
  
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

}  // namespace detail

class KMer {
 public:
  KMer() = default;

  constexpr KMer(std::uint64_t value, std::uint32_t position, bool strand)
      : value_(value), read_origin_((position << 1ULL) | strand) {}

  constexpr auto value() const noexcept -> std::uint64_t { return value_; }

  constexpr auto position() const noexcept -> std::uint64_t {
    return read_origin_ >> 1ULL;
  }

  constexpr auto strand() const noexcept -> bool { return read_origin_ & 1ULL; }

 private:
  std::uint64_t value_;
  std::uint32_t read_origin_;
};

auto Minimize(std::string const& seq, std::uint8_t const kmer_len,
              std::uint8_t const win_len) -> std::vector<KMer> {
  auto dst = std::vector<KMer>();
  auto const kMask = (1ULL << (kmer_len * 2ULL)) - 1ULL;

  auto const hash = [kMask](std::uint64_t val) -> std::uint64_t {
    // return detail::Hash(val, kMask);
    return val;
  };

  auto curr_kmer_val = std::uint64_t(0);
  auto rc_curr_kmer_val = std::uint64_t(0);
  auto window = std::deque<KMer>();

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

  auto const window_push = [&window](KMer const kmer) -> void {
    while (!window.empty() && window.back().value() > kmer.value()) {
      window.pop_back();
    }

    window.push_back(kmer);
  };

  auto const update_window = [&window, &dst](std::uint32_t const pos) -> void {
    while (!window.empty() && (window.front().position() <= pos)) {
      window.pop_front();
    }
  };

  for (auto i = 0U; i < seq.size(); ++i) {
    curr_kmer_val = shift_kmer(curr_kmer_val, seq[i]);
    rc_curr_kmer_val = shift_rc_kmer(rc_curr_kmer_val, seq[i]);
    if (i + 1U >= kmer_len) {
      if (curr_kmer_val < rc_curr_kmer_val) {
        window_push(KMer(hash(curr_kmer_val), i + 1U - kmer_len, 0));
      } else if (curr_kmer_val > rc_curr_kmer_val) {
        window_push(KMer(hash(rc_curr_kmer_val), i + 1U - kmer_len, 1));
      }
    }
    if (i >= kmer_len - 1U + win_len - 1U) {
      if (dst.empty() || dst.back().position() != window.front().position()) {
        dst.push_back(window.front());
      }
      update_window(i - (kmer_len - 1U + win_len - 1U));
    }
  }

  return dst;
}

auto DecodeKMer(std::uint64_t code, std::uint8_t kmer_len) -> std::string {
  auto dst = std::string();
  dst.reserve(kmer_len);

  while (kmer_len--) {
    dst += detail::kNucleotideDecoder[code & 3U];
    code >>= 2ULL;
  }

  std::reverse(dst.begin(), dst.end());
  return dst;
}

}  // namespace mini
namespace py = pybind11;

PYBIND11_MODULE(minipy, m) {
  py::class_<mini::KMer>(m, "KMer")
      .def(py::init<std::uint64_t, std::uint32_t, bool>())
      .def("value", &mini::KMer::value)
      .def("position", &mini::KMer::position)
      .def("strand", &mini::KMer::strand);

  m.def("minimize", &mini::Minimize);
  m.def("decode_kmer", &mini::DecodeKMer);
}
