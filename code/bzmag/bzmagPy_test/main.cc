#include "core/define.h"
#include "bzmagPy/pythonscriptserver.h"

using namespace bzmag;

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    bzmagPyScriptServer pyServer;

    String res;
    pyServer.run(String("import bzmagPy as bz"), &res);

    printf("%s", res.c_str());
    return 0;
}