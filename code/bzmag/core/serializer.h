#ifndef BZMAG_CORE_SERIALIZER_SERIALIZER_H
#define BZMAG_CORE_SERIALIZER_SERIALIZER_H
/**
    @ingroup bzmagCoreSerializer
    @class bzmag::Serializer
    @brief
*/

#include "primitivetype.h"
#include "uri.h"
#include "string.h"

namespace bzmag
{
    class Object;
    class Node;
    class Serializer
    {
    public:
        virtual bool serialize(const Uri& uri, Object* object)=0;
        virtual Object* deserialize(Node* parent, const Uri& uri, const char* name)=0;
    };
}

#endif // BZMAG_CORE_SERIALIZER_SERIALIZER_H
