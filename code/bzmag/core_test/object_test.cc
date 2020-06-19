#include "core_test/object_test.h"

#include <cassert>
#include <vector>
#include "core/type.h"
#include "core/color.h"
#include "core/autoreleasepool.h"
#include "core/object.h"
#include "core/node.h"
#include "core/module.h"
#include "core/kernel.h"
#include "core/simplepropertybinder.h"
#include "core/structproperty.h"
#include "core/enumpropertybinder.h"
#include "core/containerproperty.h"
#include "core/methodbinder.h"

using namespace bzmag;

//-----------------------------------------------------------------------------
class Super : public Object
{
public:
    DECLARE_SUPERCLASS(Super);
};
class Derived1 : public Super
{
public:
    DECLARE_CLASS(Derived1, Super);
};
class Derived2 : public Derived1
{
public:
    DECLARE_CLASS(Derived2, Derived1);
};
IMPLEMENT_SUPERCLASS(Super);
IMPLEMENT_CLASS(Derived1, Super);
IMPLEMENT_CLASS(Derived2, Derived1);
void ObjectTestCase::test_Object()
{
    Super* instance = new Derived2;
    BZMAGUNIT_ASSERT(strcmp(instance->toString(), "Derived2") == 0);
    BZMAGUNIT_ASSERT(instance->isKindOf("Super"));
    BZMAGUNIT_ASSERT(instance->isKindOf("Derived1"));
    BZMAGUNIT_ASSERT(instance->isKindOf("Derived2"));
    BZMAGUNIT_ASSERT(!instance->isKindOf("Super2"));
    BZMAGUNIT_ASSERT(instance->isKindOf(&Derived1::TYPE));
    BZMAGUNIT_ASSERT(instance->isKindOf(&Super::TYPE));\

    // Object Memory foot print
    BZMAGUNIT_ASSERT(sizeof(Object) == 12);


/*    delete instance;*/
}

//-----------------------------------------------------------------------------
class Weapon : public Object
{
public:
    DECLARE_SUPERCLASS(Weapon)
    void setPrice(const int& price)
    {
        price_ = price;
    }
    const int& getPrice() const
    {
        return price_;
    }
    void setPower(const int& power)
    {
        power_ = power;
    }
    const int& getPower() const
    {
        return power_;
    }
    void setColor(const Color& color)
    {
        color_ = color;
    }
    const Color& getColor() const
    {
        return color_;
    }
    static void bindProperty()
    {
        BIND_PROPERTY(const int&, price, &Weapon::setPrice, &Weapon::getPrice);
        BIND_PROPERTY(const int&, power, &Weapon::setPower, &Weapon::getPower);
        BIND_PROPERTY(const Color&, color, &Weapon::setColor, &Weapon::getColor);
    }

public:
    Color color_;
    int price_;
    int power_;
};
IMPLEMENT_SUPERCLASS(Weapon);

class Fighter;
class FighterMethodBinder
{
public:
    static void Fighter_i_attack_i(Fighter* self, Parameter* parameter);
    static void Fighter_v_selfHeal_i(Fighter* self, Parameter* parameter);
    static void Fighter_v_setVelocity_f(Fighter* self, Parameter* parameter);
};

class DummyNode : public Node
{

};


class Actor : public Object
{
public:
    DECLARE_SUPERCLASS(Actor)
};
class Fighter : public Actor
{
public:
    enum CharacterType
    {
        CHARACTERTYPE_FIGHTER,
        CHARACTERTYPE_SORCERESS,
        CHARACTERTYPE_HULK,
    };

public:
    DECLARE_CLASS(Fighter, Actor)
    Fighter()
    {
        dummy_ = new DummyNode;
        hp_ = 0;
    }
    virtual~Fighter()
    {
        weaponList_.clear();
    }

    int attack(int damage)
    {
        hp_ -= damage;
        return hp_;
    }
    void setVelocity(float v)
    {
        assert(v == 2.0f);
    }
    void selfHeal(int amount)
    {
        hp_ += amount;
    }

    void Hp(const int& hp)
    {
        hp_ = hp;
    }
    const int& Hp() const
    {
        return hp_;
    }
    void setName(const String& name)
    {
        name_ = name;
    }
    const String& getName() const
    {
        return name_;
    }
    void setH1(const int& h)
    {
        h_.h1_ = h;
    }
    const int& getH1() const
    {
        return h_.h1_;
    }
    void setN(const int& n)
    {
        h_.n_.n1_ = n;
    }
    const int& getN() const
    {
        return h_.n_.n1_;
    }
    void setCharacterType(const CharacterType& type)
    {
        characterType_ = type;
    }
    const CharacterType& getCharacterType() const
    {
        return characterType_;
    }
    void insertWeapon(const int& index)
    {
        weaponList_.insert(weaponList_.begin() + index, new Weapon);
    }
    void removeWeapon(const int& index)
    {
        weaponList_.erase(weaponList_.begin() + index);
    }
    Weapon*& findWeapon(const int& index)
    {
        return weaponList_[index];
    }
    void setDummy(Node* obj)
    {
        dummy_ = dynamic_cast<DummyNode*>(obj);
    }
    Node* getDummy() const
    {
        return (Node*)dummy_;
    }
    static void bindProperty()
    {
        // standard
        BIND_PROPERTY(const int&, hp, &Fighter::Hp, &Fighter::Hp);
        BIND_PROPERTY(const float&, mp, 0, 0);
        BIND_PROPERTY(const double&, str, 0, 0);
        BIND_PROPERTY(const int64_t&, dex, 0, 0);
        BIND_PROPERTY(const String&, name,
            &Fighter::setName, &Fighter::getName);

        BIND_PROPERTY(Node*, dummy, &Fighter::setDummy, &Fighter::getDummy);

        // struct hierarchy
        BIND_PROPERTY_STRUCT(test_struct);
            BIND_PROPERTY_TO_STRUCT(test_struct, const int&, h1,
                &Fighter::setH1, &Fighter::getH1);
            BIND_PROPERTY_TO_STRUCT(test_struct, const float&, h2, 0, 0);

            BIND_STRUCT_TO_STRUCT(test_struct, test_n);
                BIND_PROPERTY_TO_STRUCT(test_n, const int&, n,
                    &Fighter::setN, &Fighter::getN);

        // enum
        BIND_ENUM_PROPERTY(CharacterType, character_type,
            &Fighter::setCharacterType, &Fighter::getCharacterType);
            ENUM_PROPERTY_ADD(CharacterType, CHARACTERTYPE_FIGHTER);
            ENUM_PROPERTY_ADD(CharacterType, CHARACTERTYPE_SORCERESS);
            ENUM_PROPERTY_ADD(CharacterType, CHARACTERTYPE_HULK);

        // list
        BIND_CONTAINER_PROPERTY(int, Weapon*, weapon_list,
            &Fighter::insertWeapon, &Fighter::removeWeapon, &Fighter::findWeapon);
    }

    static void bindMethod()
    {
        BIND_METHOD(i_attack_i, &FighterMethodBinder::Fighter_i_attack_i);
        BIND_METHOD(v_selfHeal_i, &FighterMethodBinder::Fighter_v_selfHeal_i);
        BIND_METHOD(v_setVelocity_f, &FighterMethodBinder::Fighter_v_setVelocity_f);
    }

public:
    int hp_;
    float mp_;
    double str_;
    int64_t dex_;
    String name_;
    Ref<DummyNode> dummy_;

    struct Test
    {
        int h1_;
        float h2_;

        struct N
        {
            int n1_;
        };

        N n_;
    };

    Test h_;

    CharacterType characterType_;
    typedef std::vector<Weapon*>  WeaponList;
    WeaponList weaponList_;
};
IMPLEMENT_SUPERCLASS(Actor);
IMPLEMENT_CLASS(Fighter, Actor);

//-----------------------------------------------------------------------------
void FighterMethodBinder::Fighter_i_attack_i
(Fighter* self, Parameter* parameter)
{
    parameter->out()->get<int>(0) = self->attack(parameter->in()->get<int>(0));
}

//-----------------------------------------------------------------------------
void FighterMethodBinder::Fighter_v_selfHeal_i
(Fighter* self, Parameter* parameter)
{
    self->selfHeal(parameter->in()->get<int>(0));
}

//-----------------------------------------------------------------------------
void FighterMethodBinder::Fighter_v_setVelocity_f
(Fighter* self, Parameter* parameter)
{
    self->setVelocity(parameter->in()->get<float>(0));
}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_CreateObjectByConcreteType()
{
    Module module("TestModule", 0, 0);
    Module* m = &module;
    REGISTER_TYPE(m, Actor);
    REGISTER_TYPE(m, Fighter);

    Kernel* pk = Kernel::instance();


    Fighter* fighter = DOWN_CAST<Fighter*>(module.create("Fighter"));
    BZMAGUNIT_ASSERT(fighter != 0);
    BZMAGUNIT_ASSERT(fighter->isKindOf("Actor"));
    BZMAGUNIT_ASSERT(fighter->isKindOf("Fighter"));
    BZMAGUNIT_ASSERT(fighter->TYPE.getName() == "Fighter");
    BZMAGUNIT_ASSERT(fighter->isKindOf(fighter->getType()));

/*    delete fighter;*/
}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_PrimitiveTypeProperty()
{
    Module module("TestModule", 0, 0);
    Module* m = &module;
    REGISTER_TYPE(m, Actor);
    REGISTER_TYPE(m, Fighter);

    Fighter* fighter = DOWN_CAST<Fighter*>(module.create("Fighter"));
    if (!Fighter::TYPE.hasBindedProperty())
        fighter->bindProperty();

    typedef SimpleProperty<const int&> IntProperty;
    IntProperty* property =
        static_cast<IntProperty*>
        (fighter->TYPE.findProperty("hp"));

    String name = property->getTypeKeyword();
    property->set(fighter, 10);
    BZMAGUNIT_ASSERT(property->get(fighter) == 10);
    BZMAGUNIT_ASSERT(fighter->Hp() == 10);
    fighter->Hp(20);
    BZMAGUNIT_ASSERT(property->get(fighter) == 20);

    typedef std::vector<float64> vec;
    String intID = typeid(vec*).name();
    String intIDr = typeid(vec*&).name();
    String cintID = typeid(const vec*).name();
    String cintIDr = typeid(const vec*&).name();

    BZMAGUNIT_ASSERT(intID == intIDr);
    BZMAGUNIT_ASSERT(cintID == cintIDr);

/*    delete fighter;*/
}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_CustomPrimitiveTypeProperty()
{
    Module module("TestModule", 0, 0);
    Module* m = &module;
    REGISTER_TYPE(m, Actor);
    REGISTER_TYPE(m, Fighter);

    Fighter* fighter = DOWN_CAST<Fighter*>(module.create("Fighter"));
    if (!Fighter::TYPE.hasBindedProperty())
        fighter->bindProperty();

    typedef SimpleProperty<const String&> StringProperty;
    StringProperty* string_property =
        static_cast<StringProperty*>
        (fighter->TYPE.findProperty("name"));
    string_property->set(fighter, "fighter");
    BZMAGUNIT_ASSERT(string_property->get(fighter) == "fighter");
    BZMAGUNIT_ASSERT(fighter->getName() == "fighter");
    fighter->setName("fighter haha");
    BZMAGUNIT_ASSERT(string_property->get(fighter) == "fighter haha");

/*    delete fighter;*/
}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_StructTypeProperty()
{
    Module module("TestModule", 0, 0);
    Module* m = &module;
    REGISTER_TYPE(m, Actor);
    REGISTER_TYPE(m, Fighter);

    Fighter* fighter = DOWN_CAST<Fighter*>(module.create("Fighter"));
    if (!Fighter::TYPE.hasBindedProperty())
        fighter->bindProperty();

    typedef SimpleProperty<const int&> IntProperty;
    StructProperty* struct_property =
        static_cast<StructProperty*>
        (fighter->TYPE.findProperty("test_struct"));
    IntProperty* property = static_cast<IntProperty*>
        (struct_property->findProperty("h1"));
    property->set(fighter, 10);
    BZMAGUNIT_ASSERT(fighter->getH1() == 10);
    fighter->setH1(20);
    BZMAGUNIT_ASSERT(property->get(fighter) == 20);

    StructProperty* n_struct_property =
        static_cast<StructProperty*>
        (struct_property->findProperty("test_n"));
    property = static_cast<IntProperty*>
        (n_struct_property->findProperty("n"));
    property->set(fighter, 10);
    BZMAGUNIT_ASSERT(fighter->getN() == 10);
    fighter->setN(20);
    BZMAGUNIT_ASSERT(property->get(fighter) == 20);

/*    delete fighter;*/
}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_EnumTypeProperty()
{
    Module module("TestModule", 0, 0);
    Module* m = &module;
    REGISTER_TYPE(m, Actor);
    REGISTER_TYPE(m, Fighter);

    Fighter* fighter = DOWN_CAST<Fighter*>(module.create("Fighter"));
    if (!Fighter::TYPE.hasBindedProperty())
        fighter->bindProperty();

    typedef EnumProperty<Fighter::CharacterType> FighterCharacterTypeEnumProperty;
    FighterCharacterTypeEnumProperty* property =
        static_cast<FighterCharacterTypeEnumProperty*>
        (fighter->TYPE.findProperty("character_type"));

    property->set(fighter, Fighter::CHARACTERTYPE_SORCERESS);
    BZMAGUNIT_ASSERT(fighter->getCharacterType() == Fighter::CHARACTERTYPE_SORCERESS);
    fighter->setCharacterType(Fighter::CHARACTERTYPE_FIGHTER);
    BZMAGUNIT_ASSERT(property->get(fighter) == Fighter::CHARACTERTYPE_FIGHTER);
    property->set(fighter, "CHARACTERTYPE_SORCERESS");
    BZMAGUNIT_ASSERT(fighter->getCharacterType() == Fighter::CHARACTERTYPE_SORCERESS);
    property->set(fighter, "CHARACTERTYPE_FIGHTER");
    BZMAGUNIT_ASSERT(fighter->getCharacterType() == Fighter::CHARACTERTYPE_FIGHTER);

/*    delete fighter;*/
}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_ListTypeProperty()
{
    Module module("TestModule", 0, 0);
    Module* m = &module;
    REGISTER_TYPE(m, Actor);
    REGISTER_TYPE(m, Fighter);
    REGISTER_TYPE(m, Weapon);

    Fighter* fighter = DOWN_CAST<Fighter*>(module.create("Fighter"));
    if (!Fighter::TYPE.hasBindedProperty())
        fighter->bindProperty();

    typedef ContainerProperty<Fighter, int, Weapon*> WeaponListProperty;
    WeaponListProperty* property =
        static_cast<WeaponListProperty*>
        (fighter->TYPE.findProperty("weapon_list"));
    property->insert(fighter, 0);
    Weapon* weapon = property->search(fighter, 0);
    if (!Weapon::TYPE.hasBindedProperty())
        weapon->bindProperty();
    BZMAGUNIT_ASSERT(weapon);

    typedef SimpleProperty<const int&> IntProperty;
    IntProperty* price_property =
        static_cast<IntProperty*>
        (weapon->TYPE.findProperty("price"));
    price_property->set(weapon, 100);
    BZMAGUNIT_ASSERT(weapon->getPrice() == 100);

    BZMAGUNIT_ASSERT(weapon = fighter->findWeapon(0));
    BZMAGUNIT_ASSERT(weapon->getPrice() == 100);

/*    delete fighter;*/
}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_DictionaryTypeProperty()
{

}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_NodeTypeProperty()
{
    Module module("TestModule", 0, 0);
    Module* m = &module;
    REGISTER_TYPE(m, Actor);
    REGISTER_TYPE(m, Fighter);

    Fighter* fighter = DOWN_CAST<Fighter*>(module.create("Fighter"));
    if (!Fighter::TYPE.hasBindedProperty())
        fighter->bindProperty();

    // Node Property
    typedef SimpleProperty<Node*> ObjectProperty;
    ObjectProperty * object_property =
        static_cast<ObjectProperty*>
        (fighter->TYPE.findProperty("dummy"));
    Node* test = new DummyNode;
    object_property->set(fighter, test);

    String name = object_property->getName();
    String value = object_property->toString(fighter);
    String type = object_property->getTypeKeyword();
    bool readonly = object_property->isReadOnly();

    if (TypeId<Node*>::check(object_property->getType()))
    {
        Object* o = nullptr;
    }

    /*    delete fighter;*/
}

//-----------------------------------------------------------------------------
void ObjectTestCase::test_ReflectionMethod()
{
    Module module("TestModule", 0, 0);
    Module* m = &module;
    REGISTER_TYPE(m, Actor);
    REGISTER_TYPE(m, Fighter);

    Fighter* fighter = DOWN_CAST<Fighter*>(module.create("Fighter"));
    if (!Fighter::TYPE.hasBindedMethod())
        fighter->bindMethod();

    // normal method invocation
    fighter->Hp(20);
    BZMAGUNIT_ASSERT(fighter->attack(10) == 10);
    BZMAGUNIT_ASSERT(fighter->Hp() == 10);

    // reflective method invocation
    fighter->Hp(30);
    Method* attack_method = Fighter::TYPE.findMethod("attack");
    BZMAGUNIT_ASSERT(attack_method);
    attack_method->getParameter()->in()->get<int>(0) = 10;
    attack_method->invoke(fighter);
    BZMAGUNIT_ASSERT(attack_method->getParameter()->out()->get<int>(0) == 20);
    BZMAGUNIT_ASSERT(fighter->Hp() == 20);

    Method* selfheal_method = Fighter::TYPE.findMethod("selfHeal");
    BZMAGUNIT_ASSERT(selfheal_method);
    selfheal_method->getParameter()->in()->get<int>(0) = 100;
    selfheal_method->invoke(fighter);
    BZMAGUNIT_ASSERT(fighter->Hp() == 120);

    Method* setvelocity_method = Fighter::TYPE.findMethod("setVelocity");
    BZMAGUNIT_ASSERT(setvelocity_method);
    setvelocity_method->getParameter()->in()->get<float>(0) = 2.0f;
    setvelocity_method->invoke(fighter);

    Method* getproperty_method = Object::TYPE.findMethod("getProperty");
    BZMAGUNIT_ASSERT(getproperty_method);
    getproperty_method->getParameter()->in()->get<String>(0) = "dummy";
    getproperty_method->invoke(fighter);

/*    delete fighter;*/
}

//-----------------------------------------------------------------------------
class Warrior : public Node
{
public:
    DECLARE_CLASS(Warrior, Node);
};
IMPLEMENT_CLASS(Warrior, Node);
void ObjectTestCase::test_Node()
{
    Node* root = new Node();
    Node* child1 = new Node();
    Node* child2 = new Node();
    Node* child3 = new Node();
    child1->setName("test1");
    child2->setName("test2");
    child3->setName("test3");

    // reference count test
    root->attach(child1);
    BZMAGUNIT_ASSERT(child1->getRef() == 2);
    root->attach(child2);
    BZMAGUNIT_ASSERT(child2->getRef() == 2);
    child2->attach(child3);
    child1->attach(child3);
    BZMAGUNIT_ASSERT(child3->getRef() == 2);

    // rename test
    child3->setName("test3_re");
    BZMAGUNIT_ASSERT(child3->getRef() == 2);

    // noh path test
    BZMAGUNIT_ASSERT(child1->getAbsolutePath() == "/test1");
    BZMAGUNIT_ASSERT(child2->getAbsolutePath() == "/test2");
    BZMAGUNIT_ASSERT(child3->getAbsolutePath() == "/test1/test3_re");

    // Object delete
    AutoReleasePool::instance()->pump();

    root = new Node();
    Warrior* warrior1 = new Warrior();
    Warrior* warrior2 = new Warrior();
    child1 = new Node();
    child2 = new Node();
    child3 = new Node();
    warrior1->setName("warrior1");
    warrior2->setName("warrior2");
    child1->setName("test1");
    child2->setName("test2");
    child3->setName("test3");

    root->attach(warrior1);
    root->attach(warrior2);
    root->attach(child1);
    root->attach(child2);
    root->attach(child3);
    root->clearChildren<Warrior>();
    BZMAGUNIT_ASSERT(root->getNumChildren() == 3);

    AutoReleasePool::instance()->pump();
}

//-----------------------------------------------------------------------------
class Assasin : public Object
{
public:
    DECLARE_SUPERABSTRACTCLASS(Assasin);

    virtual void pureMethod() = 0;
    int hp_;
};
IMPLEMENT_SUPERABSTRACTCLASS(Assasin);
class Trapsin : public Assasin
{
public:
    DECLARE_CLASS(Trapsin, Assasin);

    virtual void pureMethod()
    {
        hp_ = 0;
    }
};
IMPLEMENT_CLASS(Trapsin, Assasin);
void ObjectTestCase::test_AbstractNode()
{
    Assasin* trapsin = new Trapsin;
    trapsin->pureMethod();
    BZMAGUNIT_ASSERT(trapsin->hp_ == 0);
/*    delete trapsin;*/
}

