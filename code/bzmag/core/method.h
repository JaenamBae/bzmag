#ifndef BZMAG_CORE_OBJECT_METHOD_H
#define BZMAG_CORE_OBJECT_METHOD_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::Method
    @brief 
*/

//#include <boost/tokenizer.hpp>
#include "primitivetype.h"
#include "string.h"
#include "parameter.h"

namespace bzmag
{
    class Object;
    class ReturnValue;
    class Method
    {
    public:
        Method();
        virtual ~Method();

        virtual void invoke(Object* self)=0;

        const String& getName();
        Parameter* getParameter();

        void setPrototype(const String& proto);
        const String& getPrototype() const;

    private:
        void build_paramter(const String& s, Variables* v);

    private:
        String prototype_;
        String name_;
        Parameter parameter_;
    };

#include "method.inl"

}

#endif // BZMAG_CORE_OBJECT_METHOD_H
