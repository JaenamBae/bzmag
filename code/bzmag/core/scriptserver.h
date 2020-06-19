#ifndef BZMAG_CORE_SCRIPT_SCRIPTSERVER_H
#define BZMAG_CORE_SCRIPT_SCRIPTSERVER_H
/**
    @ingroup bzmagCoreScriptServer
    @class bzmag::ScriptServer
    @brief
*/

#include "node.h"
#include "primitivetype.h"
#include "uri.h"

namespace bzmag
{
    class ScriptServer : public Node
    {
    public:
        DECLARE_CLASS(ScriptServer, Node);

        virtual bool run(const String& str, String* result);
        virtual bool call(
            const String& str,
            Parameter* parameter);
        virtual bool runFile(const Uri& uri, String* result);

        static void bindMethod();
    };
}

#endif // BZMAG_CORE_SCRIPT_SCRIPTSERVER_H
