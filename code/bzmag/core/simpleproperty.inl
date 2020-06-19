//-----------------------------------------------------------------------------
template <typename PROPERTY_TYPE>
type_id SimpleProperty<PROPERTY_TYPE>::getType() const
{
    return TypeId<PROPERTY_TYPE>::id();
}
