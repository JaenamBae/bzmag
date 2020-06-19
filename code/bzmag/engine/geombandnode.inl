
//-----------------------------------------------------------------------------
inline void GeomBandNode::setCenterX(const String& x)
{
    setParameters(x, scy_, sradii_, swidth_);
}

//-----------------------------------------------------------------------------
inline void GeomBandNode::setCenterY(const String& y)
{
    setParameters(scx_, y, sradii_, swidth_);
}

//-----------------------------------------------------------------------------
inline void GeomBandNode::setRadius(const String& rad)
{
    setParameters(scx_, scy_, rad, swidth_);
}

//-----------------------------------------------------------------------------
inline void GeomBandNode::setWidth(const String& width)
{
    setParameters(scx_, scy_, sradii_, width);
}

//-----------------------------------------------------------------------------
inline const String& GeomBandNode::getCenterXasString() const
{
    return scx_;
}

//-----------------------------------------------------------------------------
inline const String& GeomBandNode::getCenterYasString() const
{
    return scy_;
}

//-----------------------------------------------------------------------------
inline const String& GeomBandNode::getRadiusasString() const
{
    return sradii_;
}

//-----------------------------------------------------------------------------
inline const String& GeomBandNode::getWidthasString() const
{
    return swidth_;
}

//-----------------------------------------------------------------------------
inline String GeomBandNode::description() const
{
    return "CreateBand";
}
