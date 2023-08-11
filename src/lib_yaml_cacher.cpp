#include <Python.h>
#include <methodobject.h>
#include <object.h>
#include "YamlCacher.h"

static PyObject* py_get_yaml(PyObject* self, PyObject* args) {
    // process inputs
    char* path = NULL;
    PyObject* py_list = NULL;
    if (!PyArg_ParseTuple(args, "s|O", &path, &py_list)) {
        return NULL;
    }

    // store the keys in a vector
    std::vector<std::string> keys;
    if (py_list != NULL && PyList_Check(py_list)) {
        Py_ssize_t size = PyList_Size(py_list);
        for (Py_ssize_t i = 0; i < size; ++i) {
            PyObject* item = PyList_GetItem(py_list, i);
            if (PyUnicode_Check(item)) {
                keys.push_back(PyUnicode_AsUTF8(item));
            }
        }
    }

    return YamlCacher::get_singleton()->get_py_yaml_object(path,  keys);
}


static PyMethodDef module_methods[] = {
    {"get_yaml", py_get_yaml, METH_VARARGS, "Gets a yaml file"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module_definition = {
    PyModuleDef_HEAD_INIT,
    "yaml_cacher",
    NULL,
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_lib_yaml_cacher(void) { // PyInit_<module_name>
    return PyModule_Create(&module_definition);
}
