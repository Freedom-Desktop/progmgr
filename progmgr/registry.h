/* * * * * * * *\
	REGISTRY.H -
		Copyright (c) 2024 Vortesys, Brady McDermott
	DESCRIPTION -
		Program Manager's header file for registry related functions.
	LICENSE INFORMATION -
		MIT License, see LICENSE.txt in the root folder
\* * * * * * * */

/* Pragmas */
#pragma once

/* Defines */
#define HKEYMAXLEN 261
// Key Paths
#define WINLOGON_KEY  TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon")
// #define PROGMAN_KEY  TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Program Manager")
// #define WINDOWS_KEY  TEXT("Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows")
#define PROGMGR_KEY  TEXT("Software\\Vortesys\\Program Manager II")
// Registry configuration error values (DWORD)
#define RCE_SUCCESS 0x0000000
#define RCE_FAILURE 0x0000001
#define RCE_SETTINGS 0x0000002
#define RCE_POSITION 0x0000004
#define RCE_GROUPS 0x0000008
// Settings values (DWORD)
#define PMS_AUTOARRANGE 0x00000001
#define PMS_MINONRUN 0x00000002
#define PMS_TOPMOST 0x00000004
#define PMS_SHOWUSERNAME 0x00000008
#define PMS_SAVESETTINGS 0x00000010

/* Global Variables */
// HKEYs
extern HKEY	hKeyProgramManager;
extern HKEY	hkeyProgramGroups;
extern HKEY	hKeySettings;
// Settings
extern BOOL bAutoArrange;
extern BOOL bMinOnRun;
extern BOOL bTopMost;
extern BOOL bShowUsername;
extern BOOL bSaveSettings;
extern RECT rcMainWindow;

/* Function Prototypes */
BOOL InitializeRegistryKeys(VOID);
BOOL IsProgMgrDefaultShell(VOID);
// Groups
DWORD RegistrySaveGroup(_In_ PGROUP pg);
DWORD RegistryRemoveGroup(_In_ PGROUP pg);
// Settings
DWORD SaveConfig(_In_ BOOL bSettings, _In_ BOOL bPos, _In_ BOOL bGroups, _In_ BOOL bExit);
DWORD LoadConfig(_In_ BOOL bSettings, _In_ BOOL bPos, _In_ BOOL bGroups);
// Miscellaneous
BOOL CALLBACK SaveWindowEnumProc(HWND hWndGroup, LPARAM lParam);
