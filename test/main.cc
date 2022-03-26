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
    KMer(160962658288495797, 6, 1),  KMer(127849743288636277, 9, 0),
    KMer(120050546527532884, 16, 0), KMer(96848788933827933, 25, 0),
    KMer(134169186831835784, 33, 1), KMer(63294257164848802, 39, 1),
    KMer(89462680336978896, 43, 0),  KMer(69620345196203841, 44, 0),
    KMer(132137262280368033, 51, 0), KMer(128510159944058648, 56, 0),
    KMer(97068379428228621, 61, 1),  KMer(96138766261674208, 63, 1),
    KMer(197408025753168864, 67, 1), KMer(160962658288495797, 76, 1),
    KMer(127849743288636277, 79, 0), KMer(120050546527532884, 86, 0),
    KMer(96848788933827933, 95, 0),  KMer(134169186831835784, 103, 1)};

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
