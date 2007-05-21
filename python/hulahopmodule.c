#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pygobject.h>

void py_hulahop_register_classes (PyObject *d);

extern PyMethodDef pyhulahop_functions[];

DL_EXPORT(void)
init_hulahop(void)
{
    PyObject *m, *d;

    init_pygobject ();

    m = Py_InitModule ("_hulahop", pyhulahop_functions);
    d = PyModule_GetDict (m);

    pyhulahop_register_classes (d);

    if (PyErr_Occurred ()) {
        Py_FatalError ("can't initialise module hulahop");
    }
}
