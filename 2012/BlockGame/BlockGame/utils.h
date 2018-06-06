#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <wchar.h>

enum KEYCODE {
	NONE_KEY=0,
	LEFT_KEY=75,
	RIGHT_KEY=77,
	UP_KEY=72,
	DOWN_KEY=80,
	SPACEBAR_KEY=32,
	ENTER_KEY=13,
	ESC_KEY=27
};


typedef struct
{
	int x, y;
}Point;

typedef struct
{
	int left, top, right, bottom;
}Rect;

void OnError(const wchar_t* msg);
wchar_t* ReadFileToString(const char* filename);	// note: make sure manually free returned point