#pragma once
#include "utils.h"

void InitUI();
void ReleaseUI();

void PutChar(int x, int y, wchar_t ch);
void PutArea(wchar_t anchor, wchar_t ch);
Rect GetAnchor(wchar_t anchor);
void PrintMatrix(int x, int y, const int* matrix,
	int rows, int cols, const wchar_t* symbols);
void PrintArea(const wchar_t *tag, int x, int y);
void ClearArea(wchar_t anchor);
void ClearScreen();
void ShowText(const wchar_t* text, wchar_t anchor);
void ShowInputBox(wchar_t* text, wchar_t anchor);
int ShowMenu(const wchar_t* menus[], int num, wchar_t anchor, int style);
int WaitKey();
int DetectKey();
