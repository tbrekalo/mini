#include "mini/mini.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;

PYBIND11_MODULE(minipy, m) {
  py::class_<mini::KMer>(m, "KMer")
      .def(py::init<std::uint64_t, std::uint32_t, bool>())
      .def("value", &mini::KMer::value)
      .def("position", &mini::KMer::position)
      .def("strand", &mini::KMer::strand);

  m.def("minimize", &mini::Minimize);
}
