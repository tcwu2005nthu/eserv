#ifndef __CGI_H__
#define __CGI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "http.h"

/* add your new cgi page */
void cgi_init();

/* may be you have some memory to free*/
void cgi_uninit();

/* you can write page in C/C++ */
int cgi_handler(ExHttp *pHttp , void *handle);

/* write you own log */
int errorLog(ExHttp *pHttp , const char *mess);

typedef struct {
	char *name;
	int (*callback)(ExHttp *pHttp);
} cgi_page;

#define CGI_DECL_BEGIN \
/* customized page handler declare here */ \
cgi_page cgi_pages[] = {

#define CGI_ENTRY(NAME) \
	{ \
		.name = #NAME ".cgi" , \
		.callback = cgi_page_ ## NAME, \
	},

#define CGI_DECL_END \
};

#ifdef __cplusplus
}
#endif

#endif

