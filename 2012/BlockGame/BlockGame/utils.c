#include "utils.h"
#include <stdio.h>
#include <Windows.h>

#include "ui.h"

void OnError(const wchar_t* msg)
{
	ClearScreen();
	fwprintf(stderr, L"error: %s", msg);
	WaitKey();
	exit(1);
}

wchar_t* ReadFileToString(const char* filename)
{
	FILE* fp;
	int flen;
	char* buffer;
	wchar_t* content;
	int wchars_num;

	fp = fopen(filename, "r");
	if (!fp)
		OnError(L"open file failed");
	fseek(fp, 0, SEEK_END);
	flen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buffer = (char*)malloc(flen);
	if (!buffer)
		OnError(L"alloc memory failed");

	fread(buffer, 1, flen, fp);
	fclose(fp);

	wchars_num = MultiByteToWideChar(CP_UTF8, 0, buffer, flen, NULL, 0);
	content = (wchar_t*)malloc(wchars_num * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, buffer, flen + 1, content, wchars_num);
	free(buffer);

	return content;
}