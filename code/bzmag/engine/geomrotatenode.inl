
//-----------------------------------------------------------------------------
inline void GeomRotateNode::setAngle(const String& angle)
{
    setParameters(angle);
}

//-----------------------------------------------------------------------------
inline const String& GeomRotateNode::getAngleAsString() const
{
    return sangle_;
}

//-----------------------------------------------------------------------------
inline String GeomRotateNode::description() const
{
    return "Rotate";
}

