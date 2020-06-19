#ifndef BZMAG_CORE_BASE_TYPEID_H
#define BZMAG_CORE_BASE_TYPEID_H
/**
    @ingroup bzmagCoreBase
    @class bzmag::TypeId
    @brief 
*/

namespace bzmag
{
    template <typename T>
    class TypeId
    {
    public:
        typedef TypeId<T> type;

    public:
        static type_id id()
        {
            return reinterpret_cast<type_id>(typeid(T).name());
        }
        static bool check(type_id _id)
        {
            return id() == _id;
        }
        static bool check(const type& rhs)
        {
            return check(rhs.id());
        }
    };
}

#endif // BZMAG_CORE_BASE_TYPEID_H
