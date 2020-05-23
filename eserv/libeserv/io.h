#include "http.h"

#ifndef  __EX_IO_H__
#define  __EX_IO_H__

#ifdef __cplusplus
extern "C" {
#endif

int ex_sock_nwrite(SOCKET sock, char *buf, size_t n);

int ex_read_head(SOCKET sock , char *buf, size_t bufsize);
int ex_load_body(ExHttp *pHttp);
int sendFileStream(const ExHttp *pHttp, const char *filePath);

#ifdef __cplusplus
}
#endif

#endif

