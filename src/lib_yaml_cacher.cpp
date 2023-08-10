#include <Python.h>
#include <methodobject.h>
#include <object.h>
#include "YamlCacher.h"
long long factorial(int n) {
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

static PyObject *py_template(PyObject *self, PyObject *args) {
    int n;
    if (!PyArg_ParseTuple(args, "i", &n))
        return NULL;
    return PyLong_FromLongLong(factorial(n));
}

static PyObject* py_print_int(PyObject* self, PyObject* args) {
    int n;
    if (!PyArg_ParseTuple(args, "i", &n))
        Py_RETURN_NONE;
    printf("%d\n", n);
    Py_RETURN_NONE;
}

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
    {"template", py_template, METH_VARARGS, "description of the template funciton"},
    {"print_int", py_print_int, METH_VARARGS, "prints an integer"},
    {"get_yaml", py_get_yaml, METH_VARARGS, "Gets a yaml file"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module_definition = {
    PyModuleDef_HEAD_INIT,
    "template",
    NULL,
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_lib_yaml_cacher(void) { // PyInit_<module_name>
    return PyModule_Create(&module_definition);
}
