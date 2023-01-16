#ifndef MINI_MINI_H_
#define MINI_MINI_H_

#include <cstdint>
#include <string>
#include <vector>

namespace mini {

class KMer {
 public:
  KMer() = default;

  constexpr KMer(std::uint64_t value, std::uint64_t position, bool strand)
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
              std::uint8_t const win_len) -> std::vector<KMer>;

auto DecodeKMer(KMer const& kmer, std::uint32_t const kmer_len) -> std::string;

}  // namespace mini

#endif /* MINI_MINI_H_ */
