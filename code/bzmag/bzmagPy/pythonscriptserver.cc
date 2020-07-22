#include "python_extension.h"
#include "pythonscriptserver.h"

#include "core/kernel.h"
#include "core/module.h"
/*#include "core/resourcemanager.h"*/
#include "core/nodeeventpublisher.h"

using namespace bzmag;

IMPLEMENT_CLASS(bzmagPyScriptServer, ScriptServer);

INCLUDE_MODULE(Engine);
INCLUDE_MODULE(bzmagPy);
//INCLUDE_MODULE(WinGraphics);

//-----------------------------------------------------------------------------
static PyMethodDef bzmagPyMethods[] =
{
    { "newobj",  bzmagPy_newobject, METH_VARARGS, "create new nonamed bzObject" },
    { "new",  bzmagPy_new, METH_VARARGS, "create new bzNode" },
    { "get",  bzmagPy_get, METH_VARARGS, "get TobObject from NOH" },
    { "getObject",  bzmagPy_getObjectByID, METH_VARARGS, "get Object by ID" },
    { "delete",  bzmagPy_delete, METH_VARARGS, "delete bzNode" },
    { "ls",  bzmagPy_ls, METH_VARARGS, "list current work node" },
    { "pushcwn",  bzmagPy_pushCwn, METH_VARARGS, "push current work node" },
    { "popcwn",  bzmagPy_popCwn, METH_VARARGS, "pop current work node" },
    { "print",  bzmagPy_print, METH_VARARGS, "debug print" },
    { "exit",  bzmagPy_exit, METH_VARARGS, "exit application" },
//     { "serialize",  bzmagPy_serialize, METH_VARARGS, "serialize objects to resource" },
//     { "deserialize",  bzmagPy_deserialize, METH_VARARGS, "deserialize objects from resource" },
    { "getModuleList",  bzmagPy_getModuleList, METH_VARARGS, "get module list" },
    { "getTypeList",  bzmagPy_getTypeList, METH_VARARGS, "get type list specified module name" },
    { "getDerivedTypes",  bzmagPy_getDerivedTypes, METH_VARARGS, "get derived type list specified type name" },
    { "getTypeInfo",  bzmagPy_getTypeInfo, METH_VARARGS, "get type information specified type name" },
    { "isNode",  bzmagPy_isNode, METH_VARARGS, "specify the given type is kind of bzNodes" },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};

//-----------------------------------------------------------------------------
#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "bzmagPy",       /* m_name */
    "bzmagPy module",/* m_doc */
    -1,              /* m_size */
    bzmagPyMethods,  /* m_methods */
    NULL,            /* m_reload */
    NULL,            /* m_traverse */
    NULL,            /* m_clear */
    NULL             /* m_free */
};
#endif


//-----------------------------------------------------------------------------
// static void at_Exit()
// {
//     // if s_standAlone_ == true then this function called at exit Python
//     // SingletonServer::clear() must be called by current process(Python).
//     SingletonServer::instance()->clear();
// }


//-----------------------------------------------------------------------------
unsigned long long bzmagPyScriptServer::initialize()
{
    // initialize bzmagPy module

#if PY_MAJOR_VERSION >= 3
    Py_SetProgramName(L"bzmagPy");
    g_module = PyModule_Create(&moduledef);
#else
    Py_SetProgramName("bzmagPy");
    g_module = Py_InitModule("bzmagPy", bzmagPyMethods);
#endif

    // initialize exception handler
    PyObject* error_obj = PyErr_NewException("bzmagPy.error", 0, 0);
    Py_INCREF(error_obj);
    PyModule_AddObject(g_module, "error", error_obj);

    // initialize bzObject
    PyType_Ready(&bzObjectType);
    Py_INCREF(&bzObjectType);
    PyModule_AddObject(g_module, "bzObject", (PyObject*)(&bzObjectType));

    // initialize bzNode
    PyType_Ready(&bzNodeType);
    Py_INCREF(&bzNodeType);
    PyModule_AddObject(g_module, "bzNode", (PyObject*)(&bzNodeType));

    // initialize DynamicModules    
    USING_MODULE(Engine);
    USING_MODULE(bzmagPy);

//     if (0 == Kernel::instance()->lookup("/sys/server/script/python"))
//     {
//         bzmagPyScriptServer::setSingletonPath("/sys/server/script/python");
//         bzmagPyScriptServer::s_standAlone_ = true;
//         Py_AtExit(at_Exit);
//     }
//     else
//     {
//         bzmagPyScriptServer::setSingletonPath("/sys/server/script/python");
//         bzmagPyScriptServer::s_standAlone_ = false;
//     }
//     bzmagPyScriptServer::setSingletonPath("/sys/server/script/python");
     return (unsigned long long)g_module;
}


//-----------------------------------------------------------------------------
void bzmagPyScriptServer::NodeEventSubscriberImpl::onAttachTo
(Node* parent, Node* child)
{
}

//-----------------------------------------------------------------------------
void bzmagPyScriptServer::NodeEventSubscriberImpl::onDetachFrom
(Node* parent, Node* child)
{
}


//-----------------------------------------------------------------------------
bzmagPyScriptServer::bzmagPyScriptServer()
{
    // Test to see if we are running inside an existing Python interpreter
    if (!Py_IsInitialized())
    {
        Py_Initialize();
        initialize();
    }
}


//-----------------------------------------------------------------------------
bzmagPyScriptServer::~bzmagPyScriptServer()
{

}


//-----------------------------------------------------------------------------
bool bzmagPyScriptServer::run(const String& str, String* result)
{
    if (PyRun_SimpleString(str) == -1)
        return false;
    return true;
}


//-----------------------------------------------------------------------------
bool bzmagPyScriptServer::call(const String& str, Parameter* parameter)
{
    return false;
}


//-----------------------------------------------------------------------------
bool bzmagPyScriptServer::runFile(const Uri& uri, String* result)
{
    return false;
}


//-----------------------------------------------------------------------------
PyObject* g_module = 0;
//bzNodes g_bzobjects;
bool bzmagPyScriptServer::s_standAlone_ = false;
bzmagPyScriptServer::NodeEventSubscriberImpl
bzmagPyScriptServer::nodeEventSubscriber_;


//-----------------------------------------------------------------------------
void initialize_bzmagPy(Module* module)
{
    REGISTER_TYPE(module, bzmagPyScriptServer);
}


//-----------------------------------------------------------------------------
void finalize_bzmagPy(Module* module)
{
    if (bzmagPyScriptServer::s_standAlone_)
        Py_Finalize();
}

//-----------------------------------------------------------------------------
DECLARE_MODULE(bzmagPy);

