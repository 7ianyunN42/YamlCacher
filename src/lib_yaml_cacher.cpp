#include <Python.h>
#include <object.h>

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

static PyMethodDef module_methods[] = {
    {"template", py_template, METH_VARARGS, "description of the template funciton"},
    {"print_int", py_print_int, METH_VARARGS, "prints an integer"},
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
