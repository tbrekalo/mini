#include "mini/mini.h"
#include "nanobind/nanobind.h"
#include "nanobind/stl/string.h"
#include "nanobind/stl/vector.h"

namespace nb = nanobind;

NB_MODULE(minipy_ext, m) {
  nb::class_<mini::KMer>(m, "KMer")
      .def(nb::init<std::uint64_t, std::uint64_t, bool>())
      .def("value", &mini::KMer::value)
      .def("position", &mini::KMer::position)
      .def("strand", &mini::KMer::strand);

  m.def("minimize", mini::Minimize);
  m.def("decode_kmer", mini::DecodeKMer);
}
