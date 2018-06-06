#include "utils.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>  

#include "ui.h"

wchar_t* uidat;
Rect anchors[256];

void InitUI()
{
	CONSOLE_FONT_INFOEX cfi;
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	HANDLE console;

	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 12;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Courier New");

	setlocale(LC_ALL, "");
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleOutputCP(65001);
	SetCurrentConsoleFontEx(console, FALSE, &cfi);
	SetConsoleCursorInfo(console, &cursor_info);
	SetConsoleMode(console, ENABLE_INSERT_MODE);

	system("color F8");
	system("mode con cols=40 lines=40");
	system("TITLE BlockGame");

	memset(anchors, 0, sizeof(anchors));

	// load dat
	uidat = ReadFileToString("ui.dat");
}

void ReleaseUI()
{
	// release dat
	free(uidat);
}

void ReadArea(const wchar_t* tag, wchar_t* content, int maxsize)
{
	wchar_t starttag[256];
	wchar_t endtag[256];
	wchar_t *st, *ed;
	int contentlen;

	if (wcslen(tag) > 250)
		OnError(L"tag name is too long");
	swprintf(starttag, 256, L"{%s", tag);
	swprintf(endtag, 256, L"%s}", tag);
	st = wcsstr(uidat, starttag);
	ed = wcsstr(uidat, endtag);
	if (!st || !ed)
		OnError(L"cannot find tag");

	st += wcslen(tag) + 1;
	contentlen = ed - st;
	if (contentlen >= maxsize || contentlen <= 0)
		OnError(L"invalid area content");
	wcsncpy(content, st, contentlen);
}

void GetAnchors(wchar_t* content)
{
	int x = 0, y = 0;
	wchar_t* p = content;
	wchar_t* pre = NULL;
	wchar_t* next = p + 1;
	while (*p) {
		if (pre && *pre == L'{') {
			anchors[*p].left = x - 1;
			anchors[*p].top = y;
			*pre = L' ';
			*p = L' ';
		}
		if (*next == L'}') {
			anchors[*p].right = x + 1;
			anchors[*p].bottom = y;
			*p = L' ';
			*next = L' ';
		}
		if (*p == L'\n') {
			x = 0;
			y++;
		}
		else {
			x++;
		}
		pre = p;
		p++;
		next = p + 1;
	}
}

void GotoXY(int x, int y)
{
	COORD c;
	c.X = x, c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void PutChar(int x, int y, wchar_t ch)
{
	GotoXY(x, y);
	putwchar(ch);
}

void ClearRect(Rect area, wchar_t ch)
{
	int x, y;
	for (y = area.top; y <= area.bottom; y++) {
		for (x = area.left; x <= area.right; x++) {
			PutChar(x, y, ch);
		}
	}
}

void ClearArea(wchar_t anchor)
{
	ClearRect(anchors[anchor], L' ');
}

void PutArea(wchar_t anchor, wchar_t ch)
{
	ClearRect(anchors[anchor], ch);
}

void ClearScreen()
{
	system("cls");
	GotoXY(0, 0);
}

Rect GetAnchor(wchar_t anchor)
{
	return anchors[anchor];
}

void PrintMatrix(int x, int y, const int* matrix,
	int rows, int cols, const wchar_t* symbols)
{
	int i, j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			int elem = matrix[cols*i + j];
			wchar_t ch = symbols[elem];
			if (ch != L'\0') {
				GotoXY(x + j, y + i);
				_putwch(ch);
			}
		}
	}
}


void PrintArea(const wchar_t *tag, int x, int y)
{
	wchar_t content[4096];
	wchar_t* p = content + 1;	// remove \n at tag end
	wchar_t* line = p;
	ReadArea(tag, content, 4096);
	GetAnchors(p);
	while (1) {
		if (*p == L'\n') {
			*p = L'\0';
			GotoXY(x, y);
			wprintf(L"%s", line);
			line = p + 1;
			y++;
		}
		else if (*p == L'\0') {
			break;
		}
		p++;
	}
}

void ShowTextXY(const wchar_t* text, int x, int y)
{
	GotoXY(x, y);
	wprintf(text);
}

void ShowText(const wchar_t* text, wchar_t anchor)
{
	Rect r = anchors[anchor];
	int textlen = (int)wcslen(text);
	if (textlen > r.right - r.left + 1)
		OnError(L"text exceeded anchor");
	ShowTextXY(text, r.left, r.top);
}

void ShowInputBoxXY(wchar_t* text, int x, int y, int maxsize)
{
	wchar_t wc;
	int i = wcslen(text);
	if (maxsize >= 40)
		OnError(L"input box size too large");

	GotoXY(x, y);
	wprintf(text);

	while (1) {
		wc = _getwch();
		if (wc == L'\r')
			return;
		else if (wc == L'\b') {
			if (i > 0) {
				i--;
				text[i] = L'\0';
				GotoXY(x + i, y);
				_putwch(L' ');
			}
		}
		else if (i <= maxsize) {
			text[x + i] = wc;
			GotoXY(x + i, y);
			_putwch(wc);

			i++;
		}
	}
}

void ShowInputBox(wchar_t* text, wchar_t anchor)
{
	Rect region = anchors[anchor];
	ShowInputBoxXY(text, region.left, region.top, region.right - region.left + 1);
}

int ShowMenu(const wchar_t* menus[], int num, wchar_t anchor, int style)
{
	Rect region;
	int itemheight;
	int i, x, y, sel = 0;;
	Point cursespos[40];

	if (num >= 40)
		OnError(L"too many menu items");

	region = anchors[anchor];

	itemheight = (region.bottom - region.top - 1) / (num + 1);

	x = 5 + region.left;
	y = region.top + 1 + itemheight;

	ClearRect(region, L' ');

	for (i = 0; i < num; i++) {
		GotoXY(x, y);
		wprintf(menus[i]);
		cursespos[i].x = region.left + 2;
		cursespos[i].y = y;

		y += itemheight;
	}

	while (1)
	{
		int i;
		int c;

		for (i = 0; i < num; i++) {
			if (i == sel)
				continue;
			GotoXY(cursespos[i].x, cursespos[i].y);
			wprintf(L"  ");
		}
		GotoXY(cursespos[sel].x, cursespos[sel].y);
		wprintf(L"->");
		c = _getch();

		switch (c)
		{
		case UP_KEY:
			sel--;
			break;
		case DOWN_KEY:
			sel++;
			break;
		case ESC_KEY:
			return -1;
			break;
		case ENTER_KEY:
			return sel;
			break;
		default:;
		}
		sel = (sel + num) % num;
	}
	return -1;
}

int WaitKey()
{
	return _getwch();
}

int DetectKey()
{
	int ch = NONE_KEY;
	if (_kbhit()) {
		ch = _getch();
		if (ch == 224)
			ch = _getch();
	}
	return ch;
}
