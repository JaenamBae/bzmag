#pragma once

/*
Description : Material Node for Handling a Materials
Last Update : 2019.01.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "core/node.h"
#include "core/vector2.h"

namespace bzmag
{
namespace engine
{
    class MaterialNode : public Node
    {
    public:
        MaterialNode();
        virtual ~MaterialNode();
        DECLARE_CLASS(MaterialNode, Node);

    public:
        float64 getPermeability() const;
        float64 getConductivity() const;
        float64 getMagnetization() const;
        const Vector2& getDirectionOfMagnetization() const;
        bool isLinear() const;

        void setPermeability(float64 mu);
        void setConductivity(float64 sigma);
        void setMagnetization(float64 mag);
        void setDirectionOfMagnetization(const Vector2& dir);

    public:
        virtual bool update();
        virtual void onAttachTo(Node* parent);
        virtual void onDetachFrom(Node* parent);

    public:
        static void bindMethod();
        static void bindProperty();
    
    protected:
        void setBHCurve(const std::vector<float64>& BH_curve);
        float64 findB(float64 H) const;

    protected:
        String name_;
        bool nonlinear_;
        float64 conductivity_;
        float64 permeability_;
        float64 magnetization_;
        Vector2 dir_magnetization_;

        std::vector<float64> BH_curve_;
    };

#include "materialnode.inl"

}
}