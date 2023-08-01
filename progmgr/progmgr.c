/* * * * * * * *\
	PROGMGR.C -
		Copyright (c) 2023 freedom7341, Vortesys
	DESCRIPTION -
		Program Manager's main file, now with extra wWinMain!
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Headers */
#include "progmgr.h"
#include "resource.h"
#include "registry.h"
// #define WIN32_LEAN_AND_MEAN
#define SECURITY_WIN32
#include <Windows.h>
#include <strsafe.h>
#include <Lmcons.h>
#include <security.h>

/* Variables */
// Global
BOOL		bIsDefaultShell = FALSE;
// Global Strings
WCHAR		szAppTitle[32];
WCHAR		szProgMgr[] = L"progmgr";
WCHAR		szWebsite[64];
// Window Related
HICON		hProgMgrIcon = NULL;
HINSTANCE	hAppInstance;
HWND		hWndProgMgr = NULL;
HWND		hWndMDIClient = NULL;

/* Functions */

/* * * *\
	wWinMain -
		Program Manager's entry point.
\* * * */
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };
	HANDLE hAccel;
	HMENU hMenu;
	HMENU hSystemMenu;
	WNDCLASS wc = { 0 };
	WCHAR szBuffer[MAX_PATH];
	WCHAR szClass[16];
	WCHAR szUsername[UNLEN + 1] = L"";
	DWORD dwUsernameLen = UNLEN;
	WCHAR szWindowTitle[UNLEN + ARRAYSIZE(szAppTitle) + 4] = L"";
	RECT rcRoot;
	POINT ptOffset;

	hAppInstance = hInstance;

	// Create Strings
	LoadString(hAppInstance, IDS_PMCLASS, szClass, ARRAYSIZE(szClass));
	LoadString(hAppInstance, IDS_APPTITLE, szAppTitle, ARRAYSIZE(szAppTitle));
	LoadString(hAppInstance, IDS_WEBSITE, szWebsite, ARRAYSIZE(szWebsite));
	GetUserNameEx(NameSamCompatible, szUsername, &dwUsernameLen);

	// Get Desktop background color
	//CreateSolidBrush(GetBackgroundColor

	// Register the Frame Window
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hAppInstance;
	wc.hIcon = hProgMgrIcon = LoadImage(hAppInstance, MAKEINTRESOURCE(IDI_PROGMGR), IMAGE_ICON,
		0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszMenuName = MAKEINTRESOURCE(IDM_MAIN);
	wc.lpszClassName = szClass;
	if (!RegisterClass(&wc))
		return FALSE;

	// Load the Accelerator table
	hAccel = LoadAccelerators(hAppInstance, MAKEINTRESOURCE(IDA_ACCELS));
	if (!hAccel)
		return FALSE;

	// Perform Registry actions, close if registry is inaccessible.
	if (InitializeRegistryKeys())
	{
		bIsDefaultShell = IsProgMgrDefaultShell();
		LoadConfig();
	}
	else
	{
		return FALSE;
	}

	// Add username to window title if settings permit
	StringCchCopy(szWindowTitle, ARRAYSIZE(szAppTitle), szAppTitle);

	if (bShowUsername)
	{
		StringCchCat(szWindowTitle, ARRAYSIZE(szWindowTitle), L" - ");
		StringCchCat(szWindowTitle, ARRAYSIZE(szWindowTitle), szUsername);
	}

	// Get size of the root HWND
	GetWindowRect(GetDesktopWindow(), &rcRoot);

	// Get the initial window offset
	SystemParametersInfo(SPI_ICONHORIZONTALSPACING, 0, &ptOffset.x, 0);
	SystemParametersInfo(SPI_ICONVERTICALSPACING, 0, &ptOffset.y, 0);

	// Create main window with a default size
	// NOTE: i pulled 320x240 out of my ass, make this dynamic later
	if (!CreateWindow(wc.lpszClassName, szWindowTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		rcRoot.left + ptOffset.x, rcRoot.top + ptOffset.y,
		rcRoot.left + ptOffset.x + 320, rcRoot.top + ptOffset.y + 240,
		0, 0, hAppInstance, NULL))
		return 2;

	// Set the window size from the registry, but only if the coords make sense
	if ((rcMainWindow.left != rcMainWindow.right) && (rcMainWindow.top != rcMainWindow.bottom))
		SetWindowPos(hWndProgMgr, NULL,
			rcMainWindow.left, rcMainWindow.top,
			rcMainWindow.right - rcMainWindow.left,
			rcMainWindow.bottom - rcMainWindow.top, SWP_NOZORDER);

	// Load the menus...
	hMenu = GetMenu(hWndProgMgr);
	hSystemMenu = GetSystemMenu(hWndProgMgr, FALSE);

	// Update our menu checkmarks
	UpdateChecks(bAutoArrange, IDM_OPTIONS, IDM_OPTIONS_AUTOARRANGE);
	UpdateChecks(bMinOnRun, IDM_OPTIONS, IDM_OPTIONS_MINONRUN);
	UpdateChecks(bTopMost, IDM_OPTIONS, IDM_OPTIONS_TOPMOST);
	UpdateChecks(bShowUsername, IDM_OPTIONS, IDM_OPTIONS_SHOWUSERNAME);
	UpdateChecks(bSaveSettings, IDM_OPTIONS, IDM_OPTIONS_SAVESETTINGS);

	// Update settings based on their values
	if (bTopMost)
		SetWindowPos(hWndProgMgr, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	if (bIsDefaultShell)
	{
		// Modify the context menus since we're the default shell
		DeleteMenu(hSystemMenu, SC_CLOSE, MF_BYCOMMAND);

		LoadString(hAppInstance, IDS_SHUTDOWN, szBuffer, ARRAYSIZE(szBuffer));
		InsertMenu(hSystemMenu, 6, MF_BYPOSITION | MF_STRING, IDM_SHUTDOWN, szBuffer);
		ModifyMenu(hMenu, IDM_FILE_EXIT, MF_BYCOMMAND | MF_STRING, IDM_SHUTDOWN, szBuffer);

		LoadString(hAppInstance, IDS_RUN, szBuffer, ARRAYSIZE(szBuffer));
		InsertMenu(hSystemMenu, 6, MF_BYPOSITION | MF_STRING, IDM_FILE_RUN, szBuffer);

		LoadString(hAppInstance, IDS_TASKMGR, szBuffer, ARRAYSIZE(szBuffer));
		InsertMenu(hSystemMenu, 6, MF_BYPOSITION | MF_STRING, IDM_TASKMGR, szBuffer);
		
		// Create the desktop window...
		// CreateDesktopWindow();
	}

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!TranslateMDISysAccel(hWndMDIClient, &msg) &&
			!TranslateAccelerator(hWndProgMgr, hAccel, &msg))
		{
			DispatchMessage(&msg);
		}
	}

	return 0;
}
