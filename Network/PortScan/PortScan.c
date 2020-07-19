#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winbase.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "beacon.h"


DECLSPEC_IMPORT INT WINAPI Ws2_32$WSAGetLastError();
DECLSPEC_IMPORT INT WSAAPI Ws2_32$getaddrinfo(PCSTR,PCSTR, const ADDRINFOA*, PADDRINFOA*);
DECLSPEC_IMPORT SOCKET WSAAPI Ws2_32$socket(INT,INT,INT);
DECLSPEC_IMPORT INT WSAAPI Ws2_32$connect(SOCKET, const SOCKADDR*, INT);
DECLSPEC_IMPORT INT WINAPI Ws2_32$closesocket();
DECLSPEC_IMPORT VOID WSAAPI Ws2_32$freeaddrinfo(PADDRINFOA);
DECLSPEC_IMPORT INT KERNEL32$RtlZeroMemory(void *data, size_t size);
DECLSPEC_IMPORT INT WSAAPI Ws2_32$WSACleanup();
DECLSPEC_IMPORT INT WSAAPI Ws2_32$WSAStartup(WORD,LPWSADATA);
DECLSPEC_IMPORT INT WSAAPI Ws2_32$closesocket(IN SOCKET);



void go(char * buff, int len)  
	{
        datap parser;
        char * target;
        char * port;

        BeaconDataParse(&parser, buff, len);
        target = BeaconDataExtract(&parser, NULL);
        port = BeaconDataExtract(&parser, NULL);

        WSADATA wsaData;
        SOCKET ConnectSocket = INVALID_SOCKET;
        struct addrinfo *result = NULL,
                        *ptr = NULL,
                        hints;

        int iResult;

        // Initialize Winsock
        iResult = Ws2_32$WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            BeaconPrintf(CALLBACK_OUTPUT,"WSAStartup failed with error: %d\n", iResult);
        
        }

        KERNEL32$RtlZeroMemory( &hints, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        //
        // Resolve the server address and port
        iResult = Ws2_32$getaddrinfo(target, port, &hints, &result);
        if ( iResult != 0 ) {
            BeaconPrintf(CALLBACK_OUTPUT,"getaddrinfo failed with error: %d\n", iResult);
            Ws2_32$WSACleanup();
        
        }

        // Attempt to connect to an address until one succeeds
        for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

            // Create a SOCKET for connecting to server
            ConnectSocket = Ws2_32$socket(ptr->ai_family, ptr->ai_socktype, 
                ptr->ai_protocol);
            if (ConnectSocket == INVALID_SOCKET) {
                BeaconPrintf(CALLBACK_OUTPUT,"socket failed with error: %ld\n", Ws2_32$WSAGetLastError());
                Ws2_32$WSACleanup();
            
            } 

            // Connect to server.
            iResult = Ws2_32$connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                Ws2_32$closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        Ws2_32$freeaddrinfo(result);

        if (ConnectSocket == INVALID_SOCKET) {
            BeaconPrintf(CALLBACK_OUTPUT,"Port %s not open on %s\n", port, target);
            Ws2_32$WSACleanup();
        
        }else {
                BeaconPrintf(CALLBACK_OUTPUT, "Port %s open on %s\n", port, target);
            }

    // cleanup
    Ws2_32$closesocket(ConnectSocket);
    Ws2_32$WSACleanup();

    

}