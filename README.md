# BOF_Collection
 Various Cobalt Strike BOFs

## Requirements
[beacon.h](https://www.cobaltstrike.com/downloads/beacon.h)

For compilation instructions and BOF overview [help-bof](https://www.cobaltstrike.com/help-beacon-object-files)

## Active_Directory

### GetDomainInfo.c

Returns information on the current domain and domain controller.

`inline-execute GetDomainInfo.o`

## Collection

### GetClipboard.c

Prints any text on the clipboard.

`inline-execute GetClipboard.o`

### dumpwifi.c

Enumerates WiFi interfaces and dumps clear text credentials

load dumpwifi.cna

`enumwifi`

`dumpwifi Wifi_Profile_Name`

## Network

### PortScan.c

Scans a single port on a remote host.

load portscan.cna

`bofportscan 192.168.1.10 3389`

## Persistence

### RegistryPersistence.c

Installs or removes registry persistence.

`inline-execute RegistryPersistence.o Install`

`inline-execute RegistryPersistence.o Remove`

