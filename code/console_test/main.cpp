#include <iostream>
#include <locale>
#include "iconv/include/iconv.h"



void main()
{
//	setlocale(LC_CTYPE, "POSIX");

	char szsrc[1024] = "test_대한민국_ゴンニチと";
	wchar_t szwsrc[] = L"test_대한민국_ゴンニチと";

	wchar_t szwdest[1024] = { 0x00, };
	char szdest[1024] = { 0x00, };

	const char* src = (char*)szwsrc;
	char* dest = (char*)szdest;
	size_t src_len = strlen(src);
	size_t dest_len = 1024;

	// According to the docs Windows uses UTF-16le. 
	// This means that we should encode all strings with UTF-16le or it would not work
//	iconv_t it = iconv_open("UTF-16LE", "EUC-KR"); // EUC-KR을 UTF-16LE로
//	iconv_t it = iconv_open("wchar_t", "char"); // char을 wchar_t로

//	iconv_t it = iconv_open("EUC-KR", "UTF-16LE"); // wchar_t을 char로
	iconv_t it = iconv_open("char", "wchar_t"); // wchar_t을 char로

	if (it != (iconv_t)(-1))
	{
		int ret = iconv(it, &src, &src_len, &dest, &dest_len);
		if (ret == (size_t)(-1))
		{
			switch (errno)
			{
			case E2BIG:
				std::cout << "There is not sufficient room at *outbuf" << std::endl;
				break;

			case EILSEQ:
				std::cout << "An invalid multibyte sequence has been encountered in the input" << std::endl;
				break;

			case EINVAL:
				std::cout << "An incomplete multibyte sequence has been encountered in the input" << std::endl;
				break;

			default:
				break;
			}
		}
		else
		{
			std::cout << "KSC : " << szsrc << "->";
			std::wcout << "UTF-8 : " << szdest << std::endl;
		}
	}


	iconv_close(it);


	return;
}