//-----------------------------------------------------------------------------
inline bool String::empty() const
{
    return data_.empty();
}


//-----------------------------------------------------------------------------
inline String::size_type String::size() const
{
    return data_.size();
}


//-----------------------------------------------------------------------------
inline String::size_type String::length() const
{
    return data_.length();
}


//-----------------------------------------------------------------------------
inline const String::value_type* String::c_str() const
{
    return data_.c_str();
}


//-----------------------------------------------------------------------------
inline const String::value_type* String::data() const
{
    return data_.data();
}


//-----------------------------------------------------------------------------
inline String::reference String::at(size_type offset)
{
    return data_.at(offset);
}


//-----------------------------------------------------------------------------
inline String::const_reference String::at(size_type offset) const
{
    return data_.at(offset);
}


//-----------------------------------------------------------------------------
inline String::iterator String::begin()
{
    return data_.begin();
}


//-----------------------------------------------------------------------------
inline String::iterator String::end()
{
    return data_.end();
}


//-----------------------------------------------------------------------------
inline String::const_iterator String::begin() const
{
    return data_.begin();
}


//-----------------------------------------------------------------------------
inline String::const_iterator String::end() const
{
    return data_.end();
}


//-----------------------------------------------------------------------------
inline String::reverse_iterator String::rbegin()
{
    return data_.rbegin();
}


//-----------------------------------------------------------------------------
inline String::reverse_iterator String::rend()
{
    return data_.rend();
}


//-----------------------------------------------------------------------------
inline String::const_reverse_iterator String::rbegin() const
{
    return data_.rbegin();
}


//-----------------------------------------------------------------------------
inline String::const_reverse_iterator String::rend() const
{
    return data_.rend();
}


//-----------------------------------------------------------------------------
inline bool String::operator < (const String& s) const
{
    return data_ < s.data_;
}


//-----------------------------------------------------------------------------
inline bool String::operator > (const String& s) const
{
    return data_ > s.data_;
}


//-----------------------------------------------------------------------------
inline bool operator == (const String& s1, const String& s2)
{
    return s1.data_ == s2.data_;
}


//-----------------------------------------------------------------------------
inline bool operator != (const String& s1, const String& s2)
{
    return s1.data_ != s2.data_;
}


//-----------------------------------------------------------------------------
inline bool operator == (const String& s1, const char* s2)
{
    return s1.data_ == s2;
}


//-----------------------------------------------------------------------------
inline bool operator != (const String& s1, const char* s2)
{
    return s1.data_ != s2;
}


//-----------------------------------------------------------------------------
inline bool operator == (const char* s1, const String& s2)
{
    return s2.data_ == s1;
}


//-----------------------------------------------------------------------------
inline bool operator != (const char* s1, const String& s2)
{
    return s2.data_ != s1;
}


//-----------------------------------------------------------------------------
inline bzmag::String operator + (const String& s1, const String& s2)
{
    String r(s1);
    r += s2;
    return r;
}


//-----------------------------------------------------------------------------
inline bzmag::String operator + (const String& s1, const char* s2)
{
    String r(s1);
    r += s2;
    return r;
}


//-----------------------------------------------------------------------------
inline bzmag::String operator + (const char* s1, const String& s2)
{
    String r(s1);
    r += s2;
    return r;
}
