#include "tostring.h"

#include <strsafe.h>
#include "define.h"
#include "primitivetype.h"
#include "string.h"
#include "vector2.h"
#include "color.h"
#include "uri.h"
#include "node.h"
#include "autoreleasepool.h"
#include "stringconverter.h"
#include "property.h"
#include "simpleproperty.h"
#include "tokenizer.h"

using namespace bzmag;

//-----------------------------------------------------------------------------
class BoolStringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<bool>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "bool";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<bool> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[8];
        //_snprintf_s(buf, 8, p->get(object) ? "true" : "false");
        str_.format(p->get(object) ? "true" : "false");
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<bool> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);

        bool v = false;
        String val(value);
        if (val == "true" || val == "True")
            v = true;
        p->set(object, v);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class Int16StringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<int16>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "int16";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<int16> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[16];
        //_snprintf_s(buf, 16, "%d", p->get(object));
        str_.format("%d", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<int16> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class UnsignedInt16StringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<uint16>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "uint16";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<uint16> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[16];
        //_snprintf_s(buf, 16, "%d", p->get(object));
        str_.format("%d", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<uint16> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class IntStringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<int>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "int";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<int> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[16];
        //_snprintf_s(buf, 16, "%d", p->get(object));
        str_.format("%d", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<int> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};


//-----------------------------------------------------------------------------
class UnsignedIntStringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<unsigned int>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "uint";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<unsigned int> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[16];
        //_snprintf_s(buf, 16, "%d", p->get(object));
        str_.format("%d", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<unsigned int> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class Int32StringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<int32>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "int32";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<int32> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[16];
        //_snprintf_s(buf, 16, "%d", p->get(object));
        str_.format("%d", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<int32> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};


//-----------------------------------------------------------------------------
class UnsignedInt32StringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<uint32>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "uint32";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<uint32> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[16];
        //_snprintf_s(buf, 16, "%d", p->get(object));
        str_.format("%d", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<uint32> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class Int64StringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<int64>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "int64";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<int64> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[32];
        //_snprintf_s(buf, 32, "%I64d", p->get(object));
        str_.format("%I64d", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<int64> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class UnsignedInt64StringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<uint64>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "uint64";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<uint64> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[32];
        //_snprintf_s(buf, 32, "%I64d", p->get(object));
        str_.format("%I64d", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<uint64> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, _atoi64(value));
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class FloatStringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<float32>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "float32";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<float32> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[16];
        //_snprintf_s(buf, 16, "%.3f", p->get(object));
        str_.format("%.3f", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<float32> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};


//-----------------------------------------------------------------------------
class DoubleStringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<float64>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "float64";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<float64> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[64];
        //_snprintf_s(buf, 32, "%.3f", p->get(object));
        str_.format("%.3f", p->get(object));
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<float64> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class StringStringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<const String&>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "string";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<const String&> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        return p->get(object);
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<const String&> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }
};

//-----------------------------------------------------------------------------
class UriStringConverter : public StringStringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<const Uri&>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "uri";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<const Uri&> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        return p->get(object).get();
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<const Uri&> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        p->set(object, value);
    }
};

//-----------------------------------------------------------------------------
class Vector2StringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<const Vector2&>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "vector2";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<const Vector2&> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[48];
        const Vector2& v = p->get(object);
        //_snprintf_s(buf, 48, "%.3f %.3f", v.x_, v.y_);
        str_.format("%lf, %lf", v.x_, v.y_);
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<const Vector2&> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);

        Tokenizer tok(value.c_str(), ',');
        Vector2 v;
        int c = 0;
        for (Tokenizer::iterator i = tok.begin();
            i != tok.end(); ++i, ++c)
            v.set(c, static_cast<float>(atof(i->c_str())));
        p->set(object, v);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class ColorStringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<const Color&>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "color";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<const Color&> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        //char buf[32];
        const Color& v = p->get(object);
        //_snprintf_s(buf, 32, "%d %d %d %d", v.r_, v.g_, v.b_, v.a_);
        str_.format("%d, %d, %d, %d", v.r_, v.g_, v.b_, v.a_);
        return str_;// = buf;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<const Color&> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);

        Tokenizer tok(value.c_str(), ',');
        Tokenizer::iterator i = tok.begin();
        Color v;
        if (i != tok.end())
        {
            v.r_ = atoi((i++)->c_str());
            if (i != tok.end())
            {
                v.g_ = atoi((i++)->c_str());
                if (i != tok.end())
                {
                    v.b_ = atoi((i++)->c_str());
                    if (i != tok.end())
                        v.a_ = atoi((i++)->c_str());
                }
            }
        }

        p->set(object, v);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class ObjectStringConverter : public StringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<Object*>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "object";
    }
    virtual const String& toString(Object* object, Property* property)
    {
        typedef SimpleProperty<Object*> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        Object* v = p->get(object);

        //char buf[16] = {0};
        // nullptr이면 Object가 존재하지 않으며 이때 ID는 -1로 설정
        // 정상적인 Object의 ID는 0보다 큼
        int ID = -1;
        if (v != nullptr) ID = v->getID();
        //_snprintf_s(buf, 16, "%d", ID);
        str_.format("%d", ID);
        return str_;
    }
    virtual void fromString(Object* object, Property* property, const String& value)
    {
        typedef SimpleProperty<Object*> AdaptiveProperty;
        AdaptiveProperty* p = static_cast<AdaptiveProperty*>(property);
        int32 ID = std::stoi(value.c_str());
        Object* v = nullptr;
        if (ID >= 0) v = AutoReleasePool::instance()->find(ID);
        if (v != nullptr || ID == -1) p->set(object, v);
    }

private:
    String str_;
};

//-----------------------------------------------------------------------------
class NodeStringConverter : public ObjectStringConverter
{
public:
    virtual type_id getTypeId()
    {
        return TypeId<Node*>::id();
    }
    virtual const char* getTypeKeyword() const
    {
        return "node";
    }
};


//-----------------------------------------------------------------------------
ToString::ToString()
{
    static BoolStringConverter bool_sc;
    addConverter(&bool_sc);

    static Int16StringConverter int16_sc;
    addConverter(&int16_sc);

    static UnsignedInt16StringConverter unsignedint16_sc;
    addConverter(&unsignedint16_sc);

    static IntStringConverter int_sc;
    addConverter(&int_sc);

    static UnsignedIntStringConverter unsignedint_sc;
    addConverter(&unsignedint_sc);

    static Int32StringConverter int32_sc;
    addConverter(&int32_sc);

    static UnsignedInt32StringConverter unsignedint32_sc;
    addConverter(&unsignedint32_sc);

    static Int64StringConverter int64_sc;
    addConverter(&int64_sc);

    static UnsignedInt64StringConverter unsignedint64_sc;
    addConverter(&unsignedint64_sc);

    static FloatStringConverter float_sc;
    addConverter(&float_sc);

    static DoubleStringConverter double_sc;
    addConverter(&double_sc);

    static StringStringConverter string_sc;
    addConverter(&string_sc);

    static Vector2StringConverter vector2_sc;
    addConverter(&vector2_sc);

    static ColorStringConverter color_sc;
    addConverter(&color_sc);

    static ObjectStringConverter object_sc;
    addConverter(&object_sc);

    static NodeStringConverter node_sc;
    addConverter(&node_sc);
}


//-----------------------------------------------------------------------------
ToString::~ToString()
{
    // empty
}


//-----------------------------------------------------------------------------
void ToString::addConverter(StringConverter* sc)
{
    converters_[sc->getTypeId()] = sc;
}


//-----------------------------------------------------------------------------
const char* ToString::getTypeKeyword(const Property* property) const
{
    StringConverters::const_iterator find_iter =
        converters_.find(property->getType());
    if (converters_.end() == find_iter)
        return "";
    return find_iter->second->getTypeKeyword();
}


//-----------------------------------------------------------------------------
const String& ToString::toString(Object* object, Property* property)
{
    StringConverters::iterator find_iter =
        converters_.find(property->getType());
    if (converters_.end() == find_iter)
    {
        static String empty;
        return empty;
    }
    return find_iter->second->toString(object, property);
}


//-----------------------------------------------------------------------------
void ToString::fromString
(Object* object, Property* property, const String& value)
{
    StringConverters::iterator find_iter =
        converters_.find(property->getType());
    if (converters_.end() == find_iter)
        return;
    return find_iter->second->fromString(object, property, value);
}
