#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "appsinstalled.pb-c.h"

static PyObject* save_to_protobuf(PyObject* self, PyObject* args) {
    const char *dev_type, *dev_id;
    double lat, lon;
    PyObject *apps_list;

    // Args parsing
    if (!PyArg_ParseTuple(args, "ssddO!", &dev_type, &dev_id, &lat, &lon, &PyList_Type, &apps_list)) {
        return NULL;
    }

    // Structure initialisation
    UserApps user_apps = USER_APPS__INIT;
    user_apps.lat = lat;
    user_apps.lon = lon;

    // Copy dev_type and dev_id
    user_apps.dev_type = strdup(dev_type);
    if (!user_apps.dev_type) {
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for dev_type");
        return NULL;
    }
    user_apps.dev_id = strdup(dev_id);
    if (!user_apps.dev_id) {
        free(user_apps.dev_type);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for dev_id");
        return NULL;
    }

    // Apps quantity
    size_t apps_count = PyList_Size(apps_list);
    user_apps.n_apps = apps_count;
    user_apps.apps = malloc(sizeof(uint32_t) * apps_count);
    if (!user_apps.apps) {
        free(user_apps.dev_type);
        free(user_apps.dev_id);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate memory for apps list");
        return NULL;
    }

    // Copy Python list to C-array
    for (size_t i = 0; i < apps_count; ++i) {
        PyObject *item = PyList_GetItem(apps_list, i);
        if (!PyLong_Check(item)) {
            free(user_apps.apps);
            free(user_apps.dev_type);
            free(user_apps.dev_id);
            PyErr_SetString(PyExc_TypeError, "apps must contain integers");
            return NULL;
        }
        user_apps.apps[i] = (uint32_t)PyLong_AsUnsignedLong(item);
    }

    // Serialisation
    size_t packed_size = user_apps__get_packed_size(&user_apps);
    void *buffer = malloc(packed_size);
    if (!buffer) {
        free(user_apps.apps);
        free(user_apps.dev_type);
        free(user_apps.dev_id);
        PyErr_SetString(PyExc_MemoryError, "Failed to allocate buffer for protobuf");
        return NULL;
    }
    user_apps__pack(&user_apps, buffer);

    // File name: dev_type_dev_id.pb
    char filename[256];
    snprintf(filename, sizeof(filename), "%s_%s.pb", dev_type, dev_id);

    // Writing
    FILE *out = fopen(filename, "wb");
    if (!out) {
        free(buffer);
        free(user_apps.apps);
        free(user_apps.dev_type);
        free(user_apps.dev_id);
        PyErr_SetString(PyExc_IOError, "Cannot open output file");
        return NULL;
    }
    fwrite(buffer, packed_size, 1, out);
    fclose(out);


    // Clear buffer
    free(buffer);
    free(user_apps.apps);
    free(user_apps.dev_type);
    free(user_apps.dev_id);

    Py_RETURN_TRUE;
};

static PyObject* load_from_protobuf(PyObject* self, PyObject* args) {
    const char *filename;

    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    FILE *in = fopen(filename, "rb");
    if (!in) {
        PyErr_SetString(PyExc_IOError, "Cannot open file for reading");
        return NULL;
    }

    fseek(in, 0, SEEK_END);
    long len = ftell(in);
    rewind(in);

    void *buffer = malloc(len);
    if (!buffer) {
        fclose(in);
        PyErr_SetString(PyExc_MemoryError, "Cannot allocate memory for buffer");
        return NULL;
    }

    fread(buffer, len, 1, in);
    fclose(in);

    UserApps *user_apps = user_apps__unpack(NULL, len, buffer);
    if (!user_apps) {
        free(buffer);
        PyErr_SetString(PyExc_RuntimeError, "Failed to unpack protobuf data");
        return NULL;
    }

    PyObject *apps_list = PyList_New(user_apps->n_apps);
    for (size_t i = 0; i < user_apps->n_apps; ++i) {
        PyList_SetItem(apps_list, i, PyLong_FromUnsignedLong(user_apps->apps[i]));
    }

    PyObject *result = Py_BuildValue(
        "ssddO",  // dev_type, dev_id, lat, lon, apps
        user_apps->dev_type ? user_apps->dev_type : "unknown",  // dev_type
        user_apps->dev_id ? user_apps->dev_id : "unknown",      // dev_id
        user_apps->lat,
        user_apps->lon,
        apps_list
    );

    user_apps__free_unpacked(user_apps, NULL);
    free(buffer);

    return result;
}

static PyMethodDef ProtobufMethods[] = {
    {"save_to_protobuf", save_to_protobuf, METH_VARARGS, "Save user data to protobuf file"},
    {"load_from_protobuf", load_from_protobuf, METH_VARARGS, "Load user data from protobuf file"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef c_protobuf_module = {
    PyModuleDef_HEAD_INIT,
    "c_protobuf",
    "C Extension for protobuf",
    -1,
    ProtobufMethods
};

PyMODINIT_FUNC PyInit_c_protobuf(void) {
    return PyModule_Create(&c_protobuf_module);
}