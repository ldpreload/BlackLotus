#ifndef __INSTALL_H__
#define __INSTALL_H__

LPWSTR	GetBotFileName(PDWORD Seed);
LPWSTR	GetBotDirectory();
LPWSTR	GetBotPath();
BOOL	IsSystemInfected();
BOOL	InstallBot();

#endif //__INSTALL_H__