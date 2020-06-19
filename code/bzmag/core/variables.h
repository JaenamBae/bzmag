#ifndef BZMAG_CORE_OBJECT_VARIABLES_H
#define BZMAG_CORE_OBJECT_VARIABLES_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::Variable
    @brief
*/

#include <vector>
#include "primitivetype.h"
#include "simplevariable.h"

namespace bzmag
{
    class Variables
    {
    public:
        Variables();
        ~Variables();

        void clear();
        bool empty() const;
        size_t size() const;
        
        template <typename T>
        void add(const T& v);
        template <typename T>
        SimpleVariable<T>& get(uint32 index);
        Variable* get(uint32 index);

        void setForceList(bool enable);
        bool isForceList() const;

    private:
        typedef std::vector<Variable*> Array;

    private:
        Array array_;
        bool forceList_;
    };

#include "variables.inl"

}

#endif // BZMAG_CORE_OBJECT_VARIABLES_H
