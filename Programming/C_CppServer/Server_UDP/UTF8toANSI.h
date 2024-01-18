#pragma once
#include "CHelper.h"

//	UTF8, 16, 32, UNICODE, ANSI, ASCII
//	UTF8 -> UNICODE(widechar) -> ANSI
void UTF8to_ANSI(char buffer[], int _byteSize)
{
	// ���� �����͸� wide character�� ��ȯ

	//	���ڿ� ũ�� ����
	int wideBufferSize = MultiByteToWideChar(CP_UTF8, 0, buffer, _byteSize, nullptr, 0);
	//	ũ�⸸ŭ�� �����ڵ��� ���ڿ� ���� �Ҵ�
	wchar_t* wideBuffer = new wchar_t[wideBufferSize + 1];

	//	UTF8 -> UNICODE
	MultiByteToWideChar(CP_UTF8, 0, buffer, _byteSize, wideBuffer, wideBufferSize);
	//	NULL����
	wideBuffer[wideBufferSize] = L'\0';

	// wide char�� char�� ��ȯ
	//	�����ڵ�� �ٲ� ���ڿ��� ũ�� ����
	int ansiLength = WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, NULL, 0, NULL, NULL);
	//	�װ��� buffer�� ��ȯ�ؼ� ����( UNICODE -> ANSI )
	WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, buffer, ansiLength, NULL, NULL);
	//	NULL����
	buffer[ansiLength] = '\0';

	//	�����Ҵ� ����
	delete[] wideBuffer;
}