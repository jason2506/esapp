#include <string>
#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <esapp/segmenter.hpp>

namespace py = pybind11;

class py_list_inserter {
 public:
    explicit py_list_inserter(py::list &list)  // NOLINT(runtime/references)
        : list_(list) { /* do nothing */ }

    py_list_inserter &operator=(std::string &&s) {
        list_.append(std::move(s));
        return *this;
    }

    py_list_inserter &operator*()       { return *this; }
    py_list_inserter &operator++()      { return *this; }
    py_list_inserter &operator++(int)   { return *this; }

 private:
    py::list &list_;
};

void deprecation_warning(std::string &&msg) {
    auto exceptions = py::module::import("exceptions");
    auto warning = py::module::import("warnings");
    warning.attr("simplefilter")("always");
    warning.attr("warn")(std::move(msg), exceptions.attr("DeprecationWarning"));
    warning.attr("simplefilter")("default");
}

PYBIND11_PLUGIN(esapp_python) {
    py::module m("esapp_python");
    py::class_<esapp::segmenter>(m, "Segmenter")
        .def(py::init<>())
        .def("__init__", [](esapp::segmenter &seg, double lrv_exp) {
            deprecation_warning("use Segmenter() instead");
            new (&seg) esapp::segmenter(lrv_exp);
        })
        .def("fit", [](esapp::segmenter &seg, std::string const &s) {
            seg.fit(s.begin(), s.end());
        })
        .def("optimize", [](esapp::segmenter &seg, esapp::segmenter::size_type n_iters) {
            deprecation_warning("its functionality is already deprecated");
            seg.optimize(n_iters);
        })
        .def("segment", [](esapp::segmenter const &seg, std::string const &s) {
            py::list list;
            seg.segment(s.begin(), s.end(), py_list_inserter(list));
            return list;
        });

    return m.ptr();
}
