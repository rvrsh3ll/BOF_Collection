# BOF_Collection
 Various Cobalt Strike BOFs

## Requirements
[beacon.h](https://www.cobaltstrike.com/downloads/beacon.h)

For compilation instructions and BOF overview [help-bof](https://www.cobaltstrike.com/help-beacon-object-files)

## Active_Directory

### GetDomainInfo.c
Returns information on the current domain and domain controller.

`inline-execute GetDomainInfo.o`

## Persistence

### RegistryPersistence.c
Installs or removes registry persistence.

`inline-execute RegistryPersistence.o Install`

`inline-execute RegistryPersistence.o Remove`

