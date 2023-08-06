/* * * * * * * *\
	GROUP.C -
		Copyright (c) 2023 freedom7341, Vortesys
	DESCRIPTION -
		Group window and program item functions.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include "progmgr.h"
#include "group.h"
#include "resource.h"
#include "registry.h"
// #define WIN32_LEAN_AND_MEAN
#include <Windows.h>

/* Functions */

/* * * *\
	GroupWndProc -
		Group window procedure.
	RETURNS -
		Zero if nothing, otherwise returns the good stuff.
\* * * */
LRESULT CALLBACK GroupWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			break;
		}
    	default:
// GrpProcDefault:
			return DefMDIChildProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/* * * *\
	TempCreateGroup -
		th
	RETURNS -
		handle to window
\* * * */
HWND TempCreateGroup(HWND hMDIClient)
{
	MDICREATESTRUCT mcs;
	HWND hChild;

	mcs.szClass = szGrpClass;
	mcs.szTitle = L"";
	mcs.hOwner = hAppInstance;
	mcs.x = mcs.y = mcs.cx = mcs.cy = CW_USEDEFAULT;
	mcs.style = WS_VSCROLL | WS_HSCROLL;

	hChild = (HWND)SendMessage(hMDIClient, WM_MDICREATE, 0, (LPARAM)(LPTSTR)&mcs);
	// if (!hChild)
	// {
	//	MessageBox(hMDIClient, L"MDI Child creation failed.", L"Oh Oh...",
	//		MB_ICONEXCLAMATION | MB_OK);
	// }
	return hChild;
}