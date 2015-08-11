#include "stdafx.h"
#include "SD_Scroll.h"
#include "FormDeploy.h"

extern HWND hWndMain;
extern FormDeploy *dlgDeploy;

BOOL SD_OnInitDialog(HWND hwnd, HWND /*hwndFocus*/, LPARAM /*lParam*/)
{
    RECT rc = {};
	GetClientRect(dlgDeploy->hWnd, &rc);
	//GetClientRect(hWndDlgMain, &rc);

    SIZE sz = { rc.right - rc.left, rc.bottom - rc.top };
	//const SIZE sz = {568, 515};

    SCROLLINFO si = {};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
    si.nPos = si.nMin = 1;

    //si.nMax = sz.cx;
    //si.nPage = sz.cx;
    //SetScrollInfo(hwnd, SB_HORZ, &si, FALSE);

    si.nMax = sz.cy;
    si.nPage = sz.cy;
    //SetScrollInfo(hWndMain, SB_VERT, &si, FALSE);

    return FALSE;
}


void SD_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    if(state != SIZE_RESTORED && state != SIZE_MAXIMIZED)
        return;

    SCROLLINFO si = {};
    si.cbSize = sizeof(SCROLLINFO);

    // const int bar[] = { SB_HORZ, SB_VERT };
	const int bar[] = { SB_VERT };
    // const int page[] = { cx, cy };
	const int page[] = { cy };

    for(size_t i = 0; i < ARRAYSIZE(bar); ++i)
    {
        si.fMask = SIF_PAGE;
        si.nPage = page[i];
        SetScrollInfo(hwnd, bar[i], &si, TRUE);

        si.fMask = SIF_RANGE | SIF_POS;
        GetScrollInfo(hwnd, bar[i], &si);

        const int maxScrollPos = si.nMax - (page[i] - 1);

        // Scroll client only if scroll bar is visible and window's
        // content is fully scrolled toward right and/or bottom side.
        // Also, update window's content on maximize.
        const bool needToScroll =
            (si.nPos != si.nMin && si.nPos == maxScrollPos) ||
            (state == SIZE_MAXIMIZED);

        if(needToScroll)
        {
            SD_ScrollClient(hwnd, bar[i], si.nPos);
        }
    }
}

void SD_OnHScroll(HWND hwnd, HWND /*hwndCtl*/, UINT code, int /*pos*/)
{
    SD_OnHVScroll(hwnd, SB_HORZ, code);
}

void SD_OnVScroll(HWND hwnd, HWND /*hwndCtl*/, UINT code, int /*pos*/)
{
    SD_OnHVScroll(hwnd, SB_VERT, code);
}

void SD_OnHVScroll(HWND hwnd, int bar, UINT code)
{
    const int scrollPos = SD_GetScrollPos(hwnd, bar, code);

    if(scrollPos == -1)
        return;

    SetScrollPos(hwnd, bar, scrollPos, TRUE);
    SD_ScrollClient(hwnd, bar, scrollPos);
}

void SD_ScrollClient(HWND hwnd, int bar, int pos)
{
    static int s_prevx = 1;
    static int s_prevy = 1;

    int cx = 0;
    int cy = 0;

    int& delta = (bar == SB_HORZ ? cx : cy);
    int& prev = (bar == SB_HORZ ? s_prevx : s_prevy);

    delta = prev - pos;
    prev = pos;

    if(cx || cy)
    {
        ScrollWindow(hwnd, cx, cy, NULL, NULL);
    }
}

int SD_GetScrollPos(HWND hwnd, int bar, UINT code)
{
    SCROLLINFO si = {};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE | SIF_TRACKPOS;
    GetScrollInfo(hwnd, bar, &si);

    const int minPos = si.nMin;
    const int maxPos = si.nMax - (si.nPage - 1);

    int result = -1;

    switch(code)
    {
    case SB_LINEUP /*SB_LINELEFT*/:
        result = max(si.nPos - 1, minPos);
        break;

    case SB_LINEDOWN /*SB_LINERIGHT*/:
        result = min(si.nPos + 1, maxPos);
        break;

    case SB_PAGEUP /*SB_PAGELEFT*/:
        result = max(si.nPos - (int)si.nPage, minPos);
        break;

    case SB_PAGEDOWN /*SB_PAGERIGHT*/:
        result = min(si.nPos + (int)si.nPage, maxPos);
        break;

    case SB_THUMBPOSITION:
        // do nothing
        break;

    case SB_THUMBTRACK:
        result = si.nTrackPos;
        break;

    case SB_TOP /*SB_LEFT*/:
        result = minPos;
        break;

    case SB_BOTTOM /*SB_RIGHT*/:
        result = maxPos;
        break;

    case SB_ENDSCROLL:
        // do nothing
        break;
    }

    return result;
}