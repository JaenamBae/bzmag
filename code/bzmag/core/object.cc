#include "object.h"
#include "define.h"
#include "autoreleasepool.h"

using namespace bzmag;

IMPLEMENT_SUPERCLASS(Object);

//-----------------------------------------------------------------------------
uint32 Object::sID_ = 0;

//-----------------------------------------------------------------------------
Object::Object() : refCount_(0)
{
    ID_ = Object::sID_++;
    autorelease();
}

//-----------------------------------------------------------------------------
Object::~Object()
{

}

//-----------------------------------------------------------------------------
uint32 Object::release()
{
    if (--refCount_ == 0)
    {
        delete this;
        return 0;
    }
    return refCount_;
}

//-----------------------------------------------------------------------------
void Object::autorelease()
{
    AutoReleasePool::instance()->add(this);
}

//-----------------------------------------------------------------------------
uint32 Object::addRef()
{
    return ++refCount_;
}

//-----------------------------------------------------------------------------
uint32 Object::getRef() const
{
    return refCount_;
}

//-----------------------------------------------------------------------------
uint32 Object::getID() const
{
    return ID_;
}

//-----------------------------------------------------------------------------
bool Object::isKindOf(const String& name)
{
    return getType()->isKindOf(name);
}

//-----------------------------------------------------------------------------
bool Object::isKindOf(const Type* type)
{
    return getType()->isKindOf(type);
}

//-----------------------------------------------------------------------------
void Object::bindProperty()
{
    // empty
}

