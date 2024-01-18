#pragma once
#include "CHelper.h"

//	UTF8, 16, 32, UNICODE, ANSI, ASCII
//	UTF8 -> UNICODE(widechar) -> ANSI
void UTF8to_ANSI(char buffer[], int _byteSize)
{
	// 받은 데이터를 wide character로 변환

	//	문자열 크기 측정
	int wideBufferSize = MultiByteToWideChar(CP_UTF8, 0, buffer, _byteSize, nullptr, 0);
	//	크기만큼의 유니코드형 문자열 동적 할당
	wchar_t* wideBuffer = new wchar_t[wideBufferSize + 1];

	//	UTF8 -> UNICODE
	MultiByteToWideChar(CP_UTF8, 0, buffer, _byteSize, wideBuffer, wideBufferSize);
	//	NULL문자
	wideBuffer[wideBufferSize] = L'\0';

	// wide char를 char로 변환
	//	유니코드로 바꾼 문자열의 크기 측정
	int ansiLength = WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, NULL, 0, NULL, NULL);
	//	그것을 buffer에 변환해서 적용( UNICODE -> ANSI )
	WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, buffer, ansiLength, NULL, NULL);
	//	NULL문자
	buffer[ansiLength] = '\0';

	//	동적할당 해제
	delete[] wideBuffer;
}