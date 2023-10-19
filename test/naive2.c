#include <Python.h>

PyObject *ret_same(PyObject* a){
    Py_INCREF(a);
    return a;
}

long sum_list(PyObject *list) {

    int i, n;
    long total = 0;
    PyObject *item;
    n = PyList_Size(list);
    if (n < 0)
        return -1; /* Not a list */ /* Caller should use PyErr_Occurred() if a - 1 is returned. */
    for (i = 0; i < n; i++) { /* PyList_GetItem does not INCREF “item”. "item" is unprotected.*/
        item = PyList_GetItem(list, i); /* Can't fail */
        total += PyLong_AsLong(item);
    }
    list = ret_same(list);
    Py_DECREF(item); /* BUG! */
    return total;
}

