#ifndef __HTTP_H__
#define __HTTP_H__

#define HTTP_USER_AGENT "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; NzT)"
#define HTTP_BUFFER_SIZE 4096

/* Functions */
BOOL HttpPostRequest(
	PCHAR  Host,
	PCHAR  URI,
	PCHAR  Data,
	PCHAR* Response
);

#endif //__HTTP_H__