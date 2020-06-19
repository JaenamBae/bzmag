#include "materialnode.h"
#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(MaterialNode, Node);

//----------------------------------------------------------------------------
MaterialNode::MaterialNode() :
    name_(""), nonlinear_(false), conductivity_(0), permeability_(1),
    magnetization_(0), dir_magnetization_(0,0)
{

}

//----------------------------------------------------------------------------
MaterialNode::~MaterialNode()
{

}

//----------------------------------------------------------------------------
float64 MaterialNode::getPermeability() const
{
    return permeability_;
}

//----------------------------------------------------------------------------
float64 MaterialNode::getConductivity() const 
{
    return conductivity_;
}

//----------------------------------------------------------------------------
float64 MaterialNode::getMagnetization() const
{
    return magnetization_;
}

//----------------------------------------------------------------------------
const Vector2& MaterialNode::getDirectionOfMagnetization() const
{
    return dir_magnetization_;
}


//----------------------------------------------------------------------------
bool MaterialNode::isLinear() const 
{
    return !nonlinear_;
}

//----------------------------------------------------------------------------
void MaterialNode::setPermeability(float64 mu)
{
    permeability_ = mu;
}

//----------------------------------------------------------------------------
void MaterialNode::setConductivity(float64 sigma)
{
    conductivity_ = sigma;
}

//----------------------------------------------------------------------------
void MaterialNode::setMagnetization(float64 mag)
{
    magnetization_ = mag;
}

//----------------------------------------------------------------------------
void MaterialNode::setDirectionOfMagnetization(const Vector2& dir)
{
    dir_magnetization_ = dir;
}

//----------------------------------------------------------------------------
void MaterialNode::setBHCurve(const std::vector<float64>& BH_curve)
{
    nonlinear_ = true;
    BH_curve_.clear();
    BH_curve_.assign(BH_curve.begin(), BH_curve.end());
}

//----------------------------------------------------------------------------
float64 MaterialNode::findB(float64 H) const
{
    return 0.0;
}



bool MaterialNode::update()
{
    return true;
}

//----------------------------------------------------------------------------
void MaterialNode::onAttachTo(Node* parent)
{

}


//----------------------------------------------------------------------------
void MaterialNode::onDetachFrom(Node* parent)
{

}


//----------------------------------------------------------------------------
void MaterialNode::bindProperty()
{
    BIND_PROPERTY(float64, Permeability,
        &setPermeability,
        &getPermeability);

    BIND_PROPERTY(float64, Conductivity,
        &setConductivity,
        &getConductivity);

    BIND_PROPERTY(float64, Magnetization,
        &setMagnetization,
        &getMagnetization);

    BIND_PROPERTY(const Vector2&, Mvector,
        &setDirectionOfMagnetization, 
        &getDirectionOfMagnetization);
}

