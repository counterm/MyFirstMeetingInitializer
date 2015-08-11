#include "stdafx.h"
BOOL SD_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
void SD_OnSize(HWND hwnd, UINT state, int cx, int cy);
void SD_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void SD_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
void SD_OnHVScroll(HWND hwnd, int bar, UINT code);
void SD_ScrollClient(HWND hwnd, int bar, int pos);
int SD_GetScrollPos(HWND hwnd, int bar, UINT code);