#include <string>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <esapp/segmenter.hpp>

namespace py = pybind11;

PYBIND11_PLUGIN(esapp_python) {
    py::module m("esapp_python");
    py::class_<esapp::segmenter>(m, "Segmenter")
        .def(py::init<double>())
        .def("fit", [](esapp::segmenter &seg, std::string const &s) {
            seg.fit(s.begin(), s.end());
        })
        .def("optimize", &esapp::segmenter::optimize)
        .def("segment", [](esapp::segmenter const &seg, std::string const &s) {
            return seg.segment(s.begin(), s.end());
        });

    return m.ptr();
}
