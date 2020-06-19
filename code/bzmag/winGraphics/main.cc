#include "core/define.h"
#include "core/module.h"
#include "core/kernel.h"

#include "modelerview.h"


using namespace bzmag;
using namespace bzmag::engine;

//-----------------------------------------------------------------------------
void initialize_WinGraphics(Module* module)
{
    REGISTER_TYPE(module, ModelerView);
    ModelerView::setSingletonPath("/sys/server/modelerview");
}

//-----------------------------------------------------------------------------
void finalize_WinGraphics(Module* module)
{

}

//-----------------------------------------------------------------------------
DECLARE_MODULE(WinGraphics);
