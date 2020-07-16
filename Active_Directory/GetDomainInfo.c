#include <windows.h>
#include <stdio.h>
#include <dsgetdc.h>
#include "beacon.h"

DECLSPEC_IMPORT DWORD WINAPI NETAPI32$DsGetDcNameA(LPVOID, LPVOID, LPVOID, LPVOID, ULONG, LPVOID);
DECLSPEC_IMPORT DWORD WINAPI NETAPI32$NetApiBufferFree(LPVOID);
// https://www.cobaltstrike.com/help-beacon-object-files
void go(char * args, int alen) {
	DWORD dwRet;
	PDOMAIN_CONTROLLER_INFO pdcInfo;

	dwRet = NETAPI32$DsGetDcNameA(NULL, NULL, NULL, NULL, 0, &pdcInfo);
	if (ERROR_SUCCESS == dwRet) {
        BeaconPrintf(CALLBACK_OUTPUT, "Domain Forest Name: %s\n"
        "Domain: %s\n"
        "Domain Controller: %s\n"
        "Domain Controller Address: %s\n"
        "DC Site Name: %s\n",
        pdcInfo->DnsForestName,
        pdcInfo->DomainName,
        pdcInfo->DomainControllerName,
        pdcInfo->DomainControllerAddress,
        pdcInfo->DcSiteName);        
	}

	NETAPI32$NetApiBufferFree(pdcInfo);
}