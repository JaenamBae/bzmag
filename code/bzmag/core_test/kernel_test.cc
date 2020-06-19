#include "core_test/kernel_test.h"

#include "core/path.h"
#include "core/kernel.h"
#include "core/object.h"
#include "core/type.h"
#include "core/module.h"
#include "core/cwnguard.h"

using namespace bzmag;

//-----------------------------------------------------------------------------
class Sorceress : public Node
{
public:
    Sorceress()
    {
        CwnGuard guard(this);
    }
    DECLARE_CLASS(Sorceress, Node);
};
IMPLEMENT_CLASS(Sorceress, Node);
void initialize_module(Module* module)
{
    module->addType(Kernel::instance(), "Sorceress", &Sorceress::TYPE);
}

//-----------------------------------------------------------------------------
void KernelTestCase::test_Path()
{
    Path path("/test");
    BZMAGUNIT_ASSERT(path.isAbsolute());
    BZMAGUNIT_ASSERT(!path.isRelative());
    path = "test";
    BZMAGUNIT_ASSERT(!path.isAbsolute());
    BZMAGUNIT_ASSERT(path.isRelative());

    path = "/test1/test2/test3/test4";
    Path::iterator iter = path.begin();
    BZMAGUNIT_ASSERT(*iter == "test1");
    ++iter;
    BZMAGUNIT_ASSERT(*iter == "test2");
    ++iter;
    BZMAGUNIT_ASSERT(*iter == "test3");
    ++iter;
    BZMAGUNIT_ASSERT(*iter == "test4");
    ++iter;
}


//-----------------------------------------------------------------------------
void KernelTestCase::test_CreateObject()
{
    Module* TestModule = new Module("TestModule", initialize_module, 0);
    Kernel::instance()->addModule(TestModule);

    Sorceress* sorceress = DOWN_CAST<Sorceress*>
        (TestModule->create("Sorceress"));
    BZMAGUNIT_ASSERT(sorceress);
    delete sorceress;
    sorceress = 0;

    // creation
    sorceress = DOWN_CAST<Sorceress*>(
        Kernel::instance()->create("Sorceress",
            "/usr/scene/sorceress"));
    BZMAGUNIT_ASSERT(sorceress);
    BZMAGUNIT_ASSERT(sorceress->getAbsolutePath() == "/usr/scene/sorceress");
    BZMAGUNIT_ASSERT(sorceress->getName() == "sorceress");

    // repeat creation
    Sorceress* sorceress1 = DOWN_CAST<Sorceress*>(
        Kernel::instance()->create("Sorceress",
            "/usr/scene/sorceress"));
    BZMAGUNIT_ASSERT(sorceress == sorceress1);

    //    sorceress->detach();
}


//-----------------------------------------------------------------------------
void KernelTestCase::test_Lookup()
{
    Sorceress* sorceress = DOWN_CAST<Sorceress*>(
        Kernel::instance()->create("Sorceress",
            "/usr/scene/sorceress"));
    BZMAGUNIT_ASSERT(sorceress);
    BZMAGUNIT_ASSERT(sorceress == Kernel::instance()->lookup("/usr/scene/sorceress"));
    BZMAGUNIT_ASSERT(Kernel::instance()->lookup("/usr/scene/sorceress") ==
        Kernel::instance()->lookup("usr/scene/sorceress"));
    BZMAGUNIT_ASSERT(sorceress->getParent() == Kernel::instance()->lookup("/usr/scene"));
    BZMAGUNIT_ASSERT(sorceress->getParent() == Kernel::instance()->lookup("usr/scene"));
    BZMAGUNIT_ASSERT(Kernel::instance()->getRoot() == Kernel::instance()->lookup("/"));

    // current working node
    Node* scene = Kernel::instance()->lookup("usr/scene");
    Kernel::instance()->pushCwn(scene);
    BZMAGUNIT_ASSERT(sorceress == Kernel::instance()->lookup("sorceress"));
    BZMAGUNIT_ASSERT(Kernel::instance()->getRoot() == Kernel::instance()->lookup("/"));
    BZMAGUNIT_ASSERT(Kernel::instance()->popCwn() == scene);

    BZMAGUNIT_ASSERT(sorceress->getParent() == sorceress->relativeNode(".."));
    BZMAGUNIT_ASSERT(sorceress == Kernel::instance()->getRoot()->relativeNode("usr/scene/sorceress"));

//    sorceress->detach();
}
