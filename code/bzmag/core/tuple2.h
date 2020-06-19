#ifndef BZMAG_CORE_TYPE_TUPLE2_H
#define BZMAG_CORE_TYPE_TUPLE2_H
/**
@ingroup bzmagCoreType
@class bzmag::Tuple2
@brief
*/

namespace bzmag
{
    template <typename T>
    class Tuple2
    {
    public:
        typedef Tuple2<T> type;

    public:
        Tuple2();
        Tuple2(T x, T y);

        void clear();
        void set(T x, T y);
        void set(int i, T value)
        {
            a_[i] = value;
        }
// 
//         T size() const
//         {
//             return bzmag_sqrt(x_ * x_ + y_ * y_);
//         }
// 
//         T length() const
//         {
//             return size();
//         }
// 
//         void normalize()
//         {
//             T l = length();
//             if (l == 0)
//                 return;
//             x_ /= l;
//             y_ /= l;
//         }
// 
//         float dotprod(const type& v)
//         {
//             return (x_ * v.x_ + y_ * v.y_);
//         }
// 
//         type operator - ()
//         {
//             return type(-x_, -y_);
//         }
//         const type& operator += (const type& rhs)
//         {
//             x_ += rhs.x_;
//             y_ += rhs.y_;
//             return *this;
//         }
//         const type& operator -= (const type& rhs)
//         {
//             x_ -= rhs.x_;
//             y_ -= rhs.y_;
//             return *this;
//         }
//         const type& operator *= (T v)
//         {
//             x_ *= v;
//             y_ *= v;
//             return *this;
//         }
// 
//         type operator + (const type& v) const
//         {
//             return type(x_ + v.x_, y_ + v.y_);
//         }
// 
//         type operator - (const type& v) const
//         {
//             return type(x_ - v.x_, y_ - v.y_);
//         }
// 
//         type operator * (float value) const
//         {
//             return type(x_ * value, y_ * value);
//         }

        const T& operator [] (int i) const
        {
            return a_[i];
        }
        T& operator [] (int i)
        {
            return a_[i];
        }

    public:
        union
        {
            struct
            {
                T x_, y_;
            };
            T a_[2];
        };
    };

#include "tuple2.inl"

}

#endif // BZMAG_CORE_TYPE_TUPLE2_H
