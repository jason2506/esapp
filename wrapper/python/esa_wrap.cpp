#include <Python.h>

#include <string>
#include <vector>

#include "structmember.h"
#include "seg.hpp"

typedef struct
{
    PyObject_HEAD
    esa::Segmenter *segmenter;
} _Segmenter;

static PyObject *new_segmenter(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    _Segmenter *self = (_Segmenter *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

static int init_segmenter(_Segmenter *self, PyObject *args, PyObject *kwds)
{
    double lrv_exp;
    size_t max_iters = 10;
    size_t max_length = 30;
    double smooth = 0.0;
    static char *kwlist[] = {
        "lrv_exp",
        "max_iters",
        "max_length",
        "smooth",
        NULL
    };

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "d|iid", kwlist, 
                                     &lrv_exp, &max_iters, &max_length, &smooth))
    {
        return -1;
    }

    self->segmenter = new esa::Segmenter(lrv_exp, max_iters, max_length, smooth);
    if (!self->segmenter) { return -1; }

    return 0;
}

static void dealloc_segmenter(_Segmenter *self)
{
    delete self->segmenter;
    self->ob_type->tp_free((PyObject *)self);
}

static PyObject *fit_segmenter(_Segmenter *self, PyObject *args)
{
    PyObject *list_obj;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &list_obj))
    {
        return NULL;
    }

    Py_ssize_t list_size = PyList_Size(list_obj);
    if (list_size < 0)
    {
        return NULL;
    }

    std::vector<std::wstring> sequences(list_size);
    for (Py_ssize_t i = 0; i < list_size; ++i)
    {
        PyObject *unicode_obj = PyList_GetItem(list_obj, i);
        Py_ssize_t unicode_size = PyUnicode_GetSize(unicode_obj);

        sequences[i].assign(unicode_size, L'0');
        PyUnicode_AsWideChar((PyUnicodeObject *)unicode_obj, &sequences[i][0], unicode_size);
    }

    self->segmenter->fit(sequences);
    return Py_None;
}

static PyObject *segment_by_segmenter(_Segmenter *self, PyObject *args)
{
    PyObject *unicode_obj;
    if (!PyArg_ParseTuple(args, "U", &unicode_obj))
    {
        return NULL;
    }

    Py_ssize_t unicode_size = PyUnicode_GetSize(unicode_obj);
    std::wstring sequence(unicode_size, L'\0');
    PyUnicode_AsWideChar((PyUnicodeObject *)unicode_obj, &sequence[0], unicode_size);
    std::vector<std::wstring> result = self->segmenter->segment(sequence);

    size_t result_size = result.size();
    PyObject *list_obj = PyList_New(result_size);
    for (size_t i = 0; i < result_size; ++i)
    {
        PyObject *unicode_obj = PyUnicode_FromWideChar(result[i].c_str(), result[i].size());
        PyList_SetItem(list_obj, i, unicode_obj);
    }

    return list_obj;
}

static PyMemberDef segmenter_members[] = {
    {NULL}  // Sentinel
};

static PyMethodDef segmenter_methods[] = {
    {"fit", (PyCFunction)fit_segmenter, METH_VARARGS, ""},
    {"segment", (PyCFunction)segment_by_segmenter, METH_VARARGS, ""},
    {NULL}  // Sentinel
};

static PyTypeObject SegmenterType = {
    PyObject_HEAD_INIT(NULL)
    0,                              // ob_size
    "esa.Segmenter",                // tp_name
    sizeof(_Segmenter),             // tp_basicsize
    0,                              // tp_itemsize
    (destructor)dealloc_segmenter,  // tp_dealloc
    0,                              // tp_print
    0,                              // tp_getattr
    0,                              // tp_setattr
    0,                              // tp_compare
    0,                              // tp_repr
    0,                              // tp_as_number
    0,                              // tp_as_sequence
    0,                              // tp_as_mapping
    0,                              // tp_hash/
    0,                              // tp_call
    0,                              // tp_str
    0,                              // tp_getattro
    0,                              // tp_setattro
    0,                              // tp_as_buffer
    Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE,            // tp_flags
    "",                             // tp_doc
    0,                              // tp_traverse
    0,                              // tp_clear
    0,                              // tp_richcompare
    0,                              // tp_weaklistoffset
    0,                              // tp_iter
    0,                              // tp_iternext
    segmenter_methods,              // tp_methods
    segmenter_members,              // tp_members
    0,                              // tp_getset
    0,                              // tp_base
    0,                              // tp_dict
    0,                              // tp_descr_get
    0,                              // tp_descr_set
    0,                              // tp_dictoffset
    (initproc)init_segmenter,       // tp_init
    0,                              // tp_alloc
    new_segmenter,                  // tp_new
};

static PyMethodDef module_methods[] = {
    {NULL}  // Sentinel
};

#ifndef PyMODINIT_FUNC  // declarations for DLL import/export
#define PyMODINIT_FUNC void
#endif

PyMODINIT_FUNC initesa(void)
{
    PyObject* module;
    if (PyType_Ready(&SegmenterType) < 0) { return;}

    module = Py_InitModule3("esa", module_methods, "");
    if (module == NULL) { return; }

    Py_INCREF(&SegmenterType);
    PyModule_AddObject(module, "Segmenter", (PyObject *)&SegmenterType);
}
