#include <algorithm>
#include <array>

#include "catch2/catch_test_macros.hpp"
#include "mini/mini.h"

namespace mini {
namespace test {

constexpr auto kTestSeq =
    "AACCTTGGACTACGATCGGGGGRACCCCGAACATCTCCTCTCCCATTCTCCCTCCCCTAGAGATTCATTC"
    "AACCTTGGACTACGATCGGGGGRACCCCGAACATCTCCTCTCCCATTCTCCCTCCCCTAGAGATTCATTC";

constexpr auto kTestSeqLen = 140U;

constexpr auto kKMerLen = 29U;
constexpr auto kWinLen = 9U;

constexpr auto kTestMinimizers = std::array<KMer, 18U>{
    KMer(36269681106532217, 6, 1),  KMer(24880002093386105, 9, 0),
    KMer(12372493052543089, 16, 0), KMer(4015999444112461, 25, 0),
    KMer(34380640922163105, 33, 1), KMer(68665488426213272, 39, 1),
    KMer(64870379786710710, 43, 0), KMer(20644342140563489, 44, 0),
    KMer(28346801699364282, 51, 0), KMer(39736456831455692, 56, 0),
    KMer(29648685663207134, 61, 1), KMer(36027946598956616, 63, 1),
    KMer(66205828659723398, 67, 1), KMer(36269681106532217, 76, 1),
    KMer(24880002093386105, 79, 0), KMer(12372493052543089, 86, 0),
    KMer(4015999444112461, 95, 0),  KMer(34380640922163105, 103, 1)};

}  // namespace test
}  // namespace mini

TEST_CASE("Minimize", "[minimize]") {
  auto const minimizers = mini::Minimize(
      mini::test::kTestSeq, mini::test::kKMerLen, mini::test::kWinLen);

  REQUIRE(minimizers.size() == mini::test::kTestMinimizers.size());
  for (auto i = 0U; i < minimizers.size(); ++i) {
    CHECK(minimizers[i].value() == mini::test::kTestMinimizers[i].value());
    CHECK(minimizers[i].position() ==
          mini::test::kTestMinimizers[i].position());
    CHECK(minimizers[i].strand() == mini::test::kTestMinimizers[i].strand());
  }
}
