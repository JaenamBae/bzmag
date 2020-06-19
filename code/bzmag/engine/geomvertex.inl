
//-----------------------------------------------------------------------------
inline void GeomVertex::setX(const String& x)
{
    setParameters(x, sy_);
}

//-----------------------------------------------------------------------------
inline void GeomVertex::setY(const String& y)
{
    setParameters(sx_, y);
}

//-----------------------------------------------------------------------------
inline const String& GeomVertex::getXasString() const
{
    return sx_;
}

//-----------------------------------------------------------------------------
inline const String& GeomVertex::getYasString() const
{
    return sy_;
}
