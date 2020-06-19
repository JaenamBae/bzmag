#include <python.h>
#include "core/define.h"
#include "core/kernel.h"
#include "core/module.h"
#include "pythonscriptserver.h"

using namespace bzmag;

//-----------------------------------------------------------------------------
extern PyObject* g_module;

#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_bzmagPy(void)
#else
PyMODINIT_FUNC initbzmagPy(void)
#endif
{
    bzmagPyScriptServer::initialize();

#if PY_MAJOR_VERSION >= 3
    return g_module;
#endif
}


