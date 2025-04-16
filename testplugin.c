#include <Python.h>

static PyObject* testplugin_add(PyObject* self, PyObject* args) {
    int a,b;
    if (!PyArg_ParseTuple(args, "ii", &a, &b)) {
        return NULL;
    }

    return PyLong_FromLong(a+b);
};

static PyMethodDef TestPluginMethods[] = {
    {"add", testplugin_add, METH_VARARGS, "add two numbers"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef examplemodule = {
    PyModuleDef_HEAD_INIT,
    "testplugin",
    "simple c plugin",
    -1,
    TestPluginMethods
};

PyMODINIT_FUNC PyInit_testplugin(void) {
    return PyModule_Create(&examplemodule);
}