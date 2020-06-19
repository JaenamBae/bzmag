//-----------------------------------------------------------------------------
template <typename PROPERTY_TYPE>
bool CompositeProperty<PROPERTY_TYPE>::addProperty(Property* property)
{
    std::pair<Properties::iterator, bool> result =
        properties_.insert(Properties::value_type(
        property->getName(), property));
    return result.second;
}


//-----------------------------------------------------------------------------
template <typename PROPERTY_TYPE>
Property* CompositeProperty<PROPERTY_TYPE>::findProperty
(const String& name)
{
    Properties::const_iterator find_iter =
        properties_.find(name);
    if (properties_.end() == find_iter)
        return 0;
    return find_iter->second;
}


//-----------------------------------------------------------------------------
template <typename PROPERTY_TYPE>
size_t CompositeProperty<PROPERTY_TYPE>::getPropertySize() const
{
    return properties_.size();
}


//-----------------------------------------------------------------------------
template <typename PROPERTY_TYPE>
Properties::iterator CompositeProperty<PROPERTY_TYPE>::firstProperty()
{
    return properties_.begin();
}


//-----------------------------------------------------------------------------
template <typename PROPERTY_TYPE>
Properties::iterator CompositeProperty<PROPERTY_TYPE>::lastProperty()
{
    return properties_.end();
}


//-----------------------------------------------------------------------------
template <typename PROPERTY_TYPE>
Properties::const_iterator CompositeProperty<PROPERTY_TYPE>::firstProperty() const
{
    return properties_.begin();
}


//-----------------------------------------------------------------------------
template <typename PROPERTY_TYPE>
Properties::const_iterator CompositeProperty<PROPERTY_TYPE>::lastProperty() const
{
    return properties_.end();
}
