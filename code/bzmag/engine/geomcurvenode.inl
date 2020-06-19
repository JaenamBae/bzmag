
//-----------------------------------------------------------------------------
inline void GeomCurveNode::setStartPoint(const String& start)
{
    setParameters(start, send_, smid_);
}

//-----------------------------------------------------------------------------
inline void GeomCurveNode::setEndPoint(const String& end)
{
    setParameters(sstart_, end, smid_);
}

//-----------------------------------------------------------------------------
inline void GeomCurveNode::setMidPoint(const String& center)
{
    setParameters(sstart_, send_, center);
}

//-----------------------------------------------------------------------------
inline const String& GeomCurveNode::getStartPoint() const
{
    return sstart_;
}

//-----------------------------------------------------------------------------
inline const String& GeomCurveNode::getEndPoint() const
{
    return send_;
}

//-----------------------------------------------------------------------------
inline const String& GeomCurveNode::getMidPoint() const
{
    return smid_;
}


//-----------------------------------------------------------------------------
inline String GeomCurveNode::description() const
{
    return "CreateCurve";
}
