#include "string.h"

#include <stdio.h>
#include <stdarg.h>
#include <locale>
#include <iostream>
#include <sstream>


// iconv library ������ 1.14�� ���׷��̵� ��
// Last Update : 2017.06.19
#include "iconv/include/iconv.h"

using namespace bzmag;


//-----------------------------------------------------------------------------
String::String(const char* s, ...)
{
    va_list args;
    va_start(args, s);
    format(s, args);
}

//-----------------------------------------------------------------------------
String::String(const wchar_t* s, ...)
{
    va_list args;
    va_start(args, s);
    format(s, args);
}

//-----------------------------------------------------------------------------
void String::format(const char* s, ...)
{
    va_list args;
    va_start(args, s);
    format(s, args);
}

//-----------------------------------------------------------------------------
void String::format(const char* s, va_list args)
{
    int len = BZMAG_VSCPRINTF(s, args) + 1;
    resize(len);
    BZMAG_VSNPRINTF(&data_[0], len, s, args);
    if (len > 0)
        resize(len - 1);
}

//-----------------------------------------------------------------------------
void String::format(const wchar_t* s, ...)
{
    va_list args;
    va_start(args, s);
    format(s, args);
}

//-----------------------------------------------------------------------------
void String::format(const wchar_t* s, va_list args)
{
    //While narrow strings provide vsnprintf, which makes it possible 
    //to determine the required output buffer size, there is no equivalent 
    //for wide strings(until C11's vsnwprintf_s), and in order to determine 
    //the buffer size, the program may need to call vswprintf,
    //check the result value, and reallocate a larger buffer,
    //trying again until successful.

    std::wstring temp;
    size_t len = 1;
    temp.resize(len);
    while (vswprintf(&temp[0], len, s, args) < 0)
        temp.resize(len++);;

    len = temp.length();

    // @Last Update : 2017.06.19
    // @Author : jaenam Bae (jaenam@dongyang.ac.kr)
    // @Desciptiopns: MultiByte Characterset�� �ִ� 4����Ʈ���� ������ �ִٰ� �����ؼ�
    //                ������ ũ�⸦ Unicode ���ڿ� ������ 4��� �����
    // ����) Unicode (wchar_t : UTF-16)�� 2Byte��
    //       ���� ��ȯ�� �ҽ��� ���̴� ����Ʈ������ len x 2 ��
    resize(len * 4);
    encoding((char*)temp.c_str(), len * 2, &data_[0], len * 4);

    // strlen() �Լ��� �̿��� ���� ��ȯ�� ���ڿ��� ���̷� ����� ��ȯ��
    size_t netlen = strlen(data_.c_str());
    resize(netlen);
}

//-----------------------------------------------------------------------------
int String::toInt() const
{
    return static_cast<int>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
int32 String::toInt32() const
{
    return static_cast<int>(std::stol(data_));
}

//-----------------------------------------------------------------------------
int64 String::toInt64() const
{
    return static_cast<int>(std::stoll(data_));
}

//-----------------------------------------------------------------------------
float String::toFloat() const
{
    return static_cast<float>(std::stof(data_));
}

//-----------------------------------------------------------------------------
double String::toDouble() const
{
    return static_cast<double>(std::stod(data_));
}

//-----------------------------------------------------------------------------
String String::extractPath() const
{
    String result;
    size_t p = this->rfind("/", this->size());
    if (-1 == p)
        result = *this;
    else
        result = this->substr(0, p);
    return result;
}


//-----------------------------------------------------------------------------
void String::replace(const String& src_str, const String& dst_str)
{
    size_t b = this->find(src_str);
    size_t s = 0;
    while (b != -1)
    {
        String head(this->substr(s, b));
        String tail(this->substr(b + src_str.size(), (size_t)-1));
        *this = head + dst_str + tail;

        b = this->find(src_str);
    }
}

//-----------------------------------------------------------------------------
String::operator char* ()
{
    return const_cast<char*>(data_.c_str());
}

//-----------------------------------------------------------------------------
String::operator const char* () const
{
    return data_.c_str();
}

//-----------------------------------------------------------------------------
String::operator wchar_t* ()
{
    return (wchar_t*)data_.c_str();
}

//-----------------------------------------------------------------------------
String::operator short()
{
    return static_cast<short>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
String::operator const short() const
{
    return static_cast<short>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
String::operator unsigned short()
{
    return static_cast<unsigned short>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
String::operator const unsigned short() const
{
    return static_cast<unsigned short>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
String::operator int()
{
    return static_cast<int>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
String::operator const int() const
{
    return static_cast<int>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
String::operator unsigned int()
{
    return static_cast<unsigned int>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
String::operator const unsigned int() const
{
    return static_cast<unsigned int>(std::stoi(data_));
}

//-----------------------------------------------------------------------------
String::operator int32 ()
{
    return static_cast<int32>(std::stol(data_));
}

//-----------------------------------------------------------------------------
String::operator const int32() const
{
    return static_cast<int32>(std::stol(data_));
}

//-----------------------------------------------------------------------------
String::operator uint32 ()
{
    return static_cast<uint32>(std::stol(data_));
}

//-----------------------------------------------------------------------------
String::operator const uint32() const
{
    return static_cast<uint32>(std::stol(data_));
}

//-----------------------------------------------------------------------------
String::operator int64 ()
{
    return static_cast<int64>(std::stoll(c_str()));
}

//-----------------------------------------------------------------------------
String::operator const int64() const
{
    return static_cast<int64>(std::stoll(c_str()));
}

//-----------------------------------------------------------------------------
String::operator uint64 ()
{
    return static_cast<uint64>(std::stoll(c_str()));
}

//-----------------------------------------------------------------------------
String::operator const uint64() const
{
    return static_cast<uint64>(std::stoll(c_str()));
}

//-----------------------------------------------------------------------------
String::operator float32 ()
{
    return static_cast<float32>(std::stof(data_));
}

//-----------------------------------------------------------------------------
String::operator const float32() const
{
    return static_cast<float32>(std::stof(data_));
}

//-----------------------------------------------------------------------------
String::operator float64 ()
{
    return static_cast<float64>(std::stod(data_));
}


//-----------------------------------------------------------------------------
String::operator const float64() const
{
    return static_cast<float64>(std::stod(data_));
}

//-----------------------------------------------------------------------------
String::operator const wchar_t* () const
{
    return (wchar_t*)data_.c_str();
}

//-----------------------------------------------------------------------------
void String::encoding
(const char* src, size_t src_len, char* dest, size_t dest_len)
{
    // wchar_t ���� char �� ��ȯ��
    // iconv�� system locale�� ���� �ڵ����� ���ڵ� Ÿ���� ����
    // ��) �ѱ����� ���, char�� multibyte char�� �Ǿ� E
    //     UC-KR �ڵ�� ���ڵ��Ǿ� �ִٰ� �����ϰ�
    //     wchar_t�� Unicode�� �����Ǿ� UTF-16LE�� ��ȯ��
    iconv_t hEncoding = iconv_open("char", "wchar_t");
    iconv(hEncoding, &src, &src_len, &dest, &dest_len);
    iconv_close(hEncoding);
}

