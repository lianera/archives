#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#include <wchar.h>  

#include "game.h"
#include "ui.h"

#define TETROMINO_NUM 7
#define PANEL_WIDTH 10
#define PANEL_HEIGHT 20

wchar_t CELL_SYMBOLS[2];
wchar_t TETROMINO_SYMBOLS[2];
wchar_t TETROMINO_ERASE[2];

typedef struct {
	int cell[16];
}Block;

typedef struct {
	Block block[4];
}Tetromino;

typedef struct {
	Tetromino tetrominos[TETROMINO_NUM];
	int tetrominos_weight[TETROMINO_NUM];
	wchar_t cell_symbol_0;
	wchar_t cell_symbol_1;
}Config;

typedef struct {
	int cell[PANEL_HEIGHT][PANEL_WIDTH];
}Panel;

typedef struct {
	int tetromino_id;
	int block_id;
	int x, y;
}TetrominoState;

typedef struct {
	wchar_t name[32];
	int level;
	int score;
	Panel* panel;
	TetrominoState* current_tetromino;
	TetrominoState* next_tetromino;
} UserData;

Config config;
Panel panel;
TetrominoState current_tetromino;
TetrominoState next_tetromino;
UserData user;

Block GetStateBlock(TetrominoState state)
{
	return config.tetrominos[state.tetromino_id].block[state.block_id];
}

void LoadConfig()
{
	wchar_t* dat = ReadFileToString("game.dat");
	wchar_t* p = dat;
	int i, j, k;
	p = wcsstr(p, L"tetrominos");
	for (i = 0; i < TETROMINO_NUM; i++) {
		for (j = 0; j < 4; j++) {
			for (k = 0; k < 16; k++) {
				while (*p != L'0' && *p != L'1') {
					if (*p == L'\0')
						OnError(L"no tetrominos in game file");
					p++;
				}
				config.tetrominos[i].block[j].cell[k] = (*p == L'0' ? 0 : 1);
				p++;
			}
		}
	}
	p = wcsstr(p, L"tetrominos_weight");
	while (*p != L'\n') {
		if (*p == L'\0')
			OnError(L"no tetrominos in game file");
		p++;
	}
	p++;
	for (i = 0; i < TETROMINO_NUM; i++) {
		swscanf(p, L"%d", &config.tetrominos_weight[i]);
		while (iswdigit(*p)) {
			p++;
		}
	}
	p = wcsstr(p, L"cell_symbol_0");
	p += wcslen(L"cell_symbol_0") + 1;
	config.cell_symbol_0 = *p;
	p = wcsstr(p, L"cell_symbol_1");
	p += wcslen(L"cell_symbol_1") + 1;
	config.cell_symbol_1 = *p;
	CELL_SYMBOLS[0] = config.cell_symbol_0;
	CELL_SYMBOLS[1] = config.cell_symbol_1;
	TETROMINO_SYMBOLS[0] = L'\0';
	TETROMINO_SYMBOLS[1] = config.cell_symbol_1;
	TETROMINO_ERASE[0] = L'\0';
	TETROMINO_ERASE[1] = config.cell_symbol_0;
	free(dat);
}

void GetPanelPos(int* x, int* y)
{
	Rect panelrect;
	panelrect = GetAnchor(L'b');
	*x = panelrect.left;
	*y = panelrect.top;
}

void DrawPanel()
{
	int x, y;
	GetPanelPos(&x, &y);
	PrintMatrix(x, y, (const int*)panel.cell, 
		PANEL_HEIGHT, PANEL_WIDTH, CELL_SYMBOLS);
}

void DrawUserData()
{
	wchar_t levelstr[32];
	wchar_t scorestr[32];
	ShowText(user.name, L'n');
	swprintf(levelstr, 32, L"%d", user.level + 1);
	ShowText(levelstr, L'l');
	swprintf(scorestr, 32, L"%d", user.score);
	ShowText(scorestr, L's');
}

void DrawCurrentTetromino(int erase)
{
	int px, py;
	int h;
	Block block;
	GetPanelPos(&px, &py);
	block = GetStateBlock(current_tetromino);
	h = current_tetromino.y < 0 ? -current_tetromino.y : 0;
	PrintMatrix(px + current_tetromino.x, py + current_tetromino.y + h,
		block.cell + 4*h, 4-h, 4, erase?TETROMINO_ERASE:TETROMINO_SYMBOLS);
}

void DrawNextTetromino()
{
	Rect rect;
	Block block;

	rect = GetAnchor(L't');
	block = GetStateBlock(next_tetromino);
	PrintMatrix(rect.left, rect.top, block.cell, 4, 4, CELL_SYMBOLS);
}

/*
return score
*/
int Eliminate(int level)
{
	int i, j;
	int lines = 0;
	int score_table[4];
	score_table[0] = 40 * (level + 1);
	score_table[1] = 100 * (level + 1);
	score_table[2] = 300 * (level + 1);
	score_table[3] = 1200 * (level + 1);

	i = PANEL_HEIGHT - 1;
	while (i >= 0) {
		int e = 1;
		// checkline
		for (j = PANEL_WIDTH - 1; j >= 0; j--) {
			if (!panel.cell[i][j])
				e = 0;
		}
		if (e) {	// eliminate line
			int m, n;
			for (m = i; m > 0; m--) {
				for (n = PANEL_WIDTH - 1; n >= 0; n--) {
					panel.cell[m][n] = panel.cell[m - 1][n];
				}
			}
			lines++;
			DrawPanel();
		}
		else {
			i--;
		}
	}
	if (lines > 4)
		OnError(L"eliminate wrong");
	if (lines > 0)
		return score_table[lines - 1];
	return 0;
}

/*
return
0 none, 1 left/right bump, 2 land
*/
int BumpCheck(TetrominoState state, int key)
{
	Block block = GetStateBlock(state);
	for (int i = 0; i < 16; i++) {
		int x = i % 4 + state.x;
		int y = i / 4 + state.y;
		if (!block.cell[i])
			continue;
		if (y >= PANEL_HEIGHT)
			return 2;
		if (x < 0 || x >= PANEL_WIDTH)
			return 1;
		if (panel.cell[y][x]) {
			if (key == LEFT_KEY || key == RIGHT_KEY)
				return 1;
			else
				return 2;
		}
	}
	return 0;
}

void SwitchNewTetromino()
{
	int weightsum = 0;
	int r;
	int i;

	// switch to next tetromino
	current_tetromino = next_tetromino;
	current_tetromino.x = PANEL_WIDTH / 2 - 2;
	current_tetromino.y = -1;

	// generate next tetromino
	for (i = 0; i < TETROMINO_NUM; i++) {
		weightsum += config.tetrominos_weight[i];
	}
	r = rand() % weightsum;
	for (i = 0; i < TETROMINO_NUM; i++) {
		r -= config.tetrominos_weight[i];
		if (r <= 0) {
			next_tetromino.tetromino_id = i;
			break;
		}
	}
	next_tetromino.block_id = rand() % 4;
}

// return 0 none, 1 gameover 
int OnLand()
{
	int score;
	int i;
	Block block = GetStateBlock(current_tetromino);
	if (current_tetromino.y <= 1) {
		return 1;
	}
	// apply current tetromino to panel
	for (i = 0; i < 16; i++) {
		int tx = current_tetromino.x + i % 4;
		int ty = current_tetromino.y + i / 4;
		if (block.cell[i]) {
			panel.cell[ty][tx] = 1;
		}
	}

	score = Eliminate(user.level);

	user.score += score;

	// level up check
	if (user.score >= (1200 * (int)pow(2, user.level))) {
		user.level++;
	}

	SwitchNewTetromino();
	DrawNextTetromino();
	return 0;
}


UserData NewUser(const wchar_t* name)
{
	UserData userdata;
	wcscpy(userdata.name, name);
	userdata.level = 0;
	userdata.score = 0;
	userdata.panel = &panel;
	userdata.current_tetromino = &current_tetromino;
	userdata.next_tetromino = &next_tetromino;
	return userdata;
}

void SaveData()
{
	FILE* fp;
	system("if not exist saves mkdir saves");
	wchar_t fname[256] = L"saves/";
	wcscat(fname, user.name);
	fp = _wfopen(fname, L"wb");
	if (!fp)
		OnError(L"create save file failed");
	fwrite(user.name, 1, sizeof(user.name), fp);
	fwrite(&user.level, sizeof(user.level), 1, fp);
	fwrite(&user.score, sizeof(user.score), 1, fp);
	fwrite(&current_tetromino, sizeof(current_tetromino), 1, fp);
	fwrite(&next_tetromino, sizeof(next_tetromino), 1, fp);
	fwrite(&panel, sizeof(panel), 1, fp);
	fclose(fp);
}

int LoadSave(const wchar_t* name)
{
	FILE* fp;
	wchar_t fname[256] = L"saves/";
	wcscat(fname, name);
	fp = _wfopen(fname, L"rb");
	if (!fp) {
		return 0;
	}
	fread(user.name, 1, sizeof(user.name), fp);
	fread(&user.level, sizeof(user.level), 1, fp);
	fread(&user.score, sizeof(user.score), 1, fp);
	fread(&current_tetromino, sizeof(current_tetromino), 1, fp);
	fread(&next_tetromino, sizeof(next_tetromino), 1, fp);
	fread(&panel, sizeof(panel), 1, fp);
	fclose(fp);
	return 1;
}

void InitGame()
{
	InitUI();
	LoadConfig();
}

void ReleaseGame()
{
	ReleaseUI();
}

void RedrawGamingScene()
{
	PrintArea(L"outline", 0, 0);
	DrawPanel();
	DrawCurrentTetromino(0);
	DrawNextTetromino();
	DrawUserData();
}

void DisplayWelcome()
{
	Rect coverrect;
	ClearScreen();
	PrintArea(L"welcome", 0, 0);
	coverrect = GetAnchor(L'c');
	PrintArea(L"cover", coverrect.left, coverrect.top);
	WaitKey();
	ClearArea(L'c');
}

void DisplayHelp()
{
	Rect rect;
	rect = GetAnchor(L'c');
	ClearArea(L's');
	PrintArea(L"help", rect.left, rect.top);
	WaitKey();
}

// return 0 resume, 1 helped, 2 return, 3 exit
int DisplayGamingMenu()
{
	int sel;
	wchar_t* menus[] = { L"RESUME", L"HELP", L"RETURN", L"EXIT" };
	//ClearArea(L'b');
	sel = ShowMenu(menus, 4, L'm', 0);
	switch (sel) {
	case -1: // esc -> resume
	case 0:
		RedrawGamingScene();
		break;
	case 1:
		DisplayHelp();
		RedrawGamingScene();
		break;
	}
	return sel;
}

void InputUserName(wchar_t name[])
{
	wchar_t s[64] = L"_NONAME_";
	ClearArea(L's');
	ShowText(L"YOUR NAME?", L'y');
	ShowInputBox(s, L'g');
	wcscpy(name, s);
}

void DisplayGameOver()
{
	ClearScreen();
	PrintArea(L"gameover", 0, 0);
	WaitKey();
}

// return 1 exit, 2 gameover, 3 normal return
int GameLoop()
{
	int accelerate = 0;
	int delay_acc = 0;

	RedrawGamingScene();
	while (1) {
		int gaming_menu_sel;
		int key = DetectKey();
		TetrominoState t = current_tetromino;
		int bump;
		int action = 0;

		int delay = 500 - user.level * 100;
		if (delay < 50)
			delay = 50;
		if (accelerate)
			delay = delay / 5;

		switch (key) {
		case LEFT_KEY:	// left
			t.x--;
			action = 1;
			break;
		case RIGHT_KEY: // right
			t.x++;
			action = 1;
			break;
		case UP_KEY: // step
			delay_acc = delay;
			break;
		case SPACEBAR_KEY: // rotate
			t.block_id++;
			t.block_id %= 4;
			action = 1;
			break;
		case DOWN_KEY:
			accelerate = 1;
			break;
		case ESC_KEY:
			gaming_menu_sel = DisplayGamingMenu();
			if (gaming_menu_sel == 2)	//return
				return 3;
			else if (gaming_menu_sel == 3)	// exit
				return 1;
			break;
		case NONE_KEY:
		default:		
			;
		}
		if (delay_acc >= delay) {	// fall
			t.y++;
			delay_acc = 0;
			action = 1;
		}
		if (action) {
			bump = BumpCheck(t, key);
			// bump == 1 wall
			if (bump == 2) {	// landed
				int gameover = OnLand();
				DrawUserData();
				if (gameover)
					return 2;
				// save data
				SaveData();
				accelerate = 0;
			}
			else if (bump == 0) {	// take action 
				// clear old block 
				DrawCurrentTetromino(1);
				// apply action
				current_tetromino = t;
				// draw new block
				DrawCurrentTetromino(0);

			}
		}
		Sleep(1);
		delay_acc++;
	}
}

void RunGame()
{
	wchar_t* menus[] = {L"START", L"CONTINUE", L"HELP", L"EXIT"};
	while (1) {
		int sel;
		wchar_t name[256];
		int gameret = 0;
		DisplayWelcome();
		ClearArea(L's');
		memset(&panel, 0, sizeof(Panel));
		sel = ShowMenu(menus, 4, L'm', 0);
		switch (sel) {
		case 0:
			InputUserName(name);
			user = NewUser(name);
			SwitchNewTetromino();
			SwitchNewTetromino();
			gameret = GameLoop();
			break;
		case 1:
			InputUserName(name);
			if (!LoadSave(name)) {
				ShowText(L"Load Failed", L'y');
				WaitKey();
			}
			else {
				gameret = GameLoop();

			}
			break;
		case 2:
			DisplayHelp();
			ClearArea(L's');
			break;
		case 3:
			return;
		}
		if (gameret == 1)
			return;
		else if (gameret == 2) {
			DisplayGameOver();
		}
		else if (gameret == 3) {

		}
	}
}