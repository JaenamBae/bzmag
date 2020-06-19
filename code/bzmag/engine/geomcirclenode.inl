
//-----------------------------------------------------------------------------
inline void GeomCircleNode::setCenter(const String& center)
{

    setParameters(center, sradii_);
}

//-----------------------------------------------------------------------------
inline void GeomCircleNode::setRadius(const String& radii)
{
    setParameters(scenter_, radii);
}

//-----------------------------------------------------------------------------
inline const String& GeomCircleNode::getCenter() const
{
    return scenter_;
}

//-----------------------------------------------------------------------------
inline const String& GeomCircleNode::getRadius() const
{
    return sradii_;
}

//-----------------------------------------------------------------------------
inline String GeomCircleNode::description() const
{
    return "CreateCircle";
}
