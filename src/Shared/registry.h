#ifndef __REGISTRY_H__
#define __REGISTRY_H__

enum REGISTRY_HIVE
{
	HIVE_HKEY_CURRENT_USER	= 0,
	HIVE_HKEY_LOCAL_MACHINE = 1
};

BOOL	RegistryOpenKeyEx(CONST LPWSTR KeyPath, HANDLE RegistryHandle, ACCESS_MASK AccessMask);
BOOL	RegistryReadValueEx(CONST LPWSTR KeyPath, CONST LPWSTR Name, LPWSTR* Value);
BOOL	RegistryReadValue(INT Hive, CONST LPWSTR Path, CONST LPWSTR Name, LPWSTR* Value);

#endif