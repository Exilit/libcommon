#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <errno.h>


#define RET_ERROR_INT(ec,ax)		do { push_error_stack_(__FILE__, __func__, __LINE__, ec, errno, ax); return -1; } while (0)
#define RET_ERROR_UINT(ec,ax)		do { push_error_stack_(__FILE__, __func__, __LINE__, ec, errno, ax); return 0; } while (0)
#define RET_ERROR_PTR(ec,ax)		do { push_error_stack_(__FILE__, __func__, __LINE__, ec, errno, ax); return NULL; } while (0)
#define RET_ERROR_CUST(rv,ec,ax)	do { push_error_stack_(__FILE__, __func__, __LINE__, ec, errno, ax); return rv; } while (0)

#define RET_ERROR_INT_FMT(ec,fmt,...)	do { push_error_stack_fmt_(__FILE__, __func__, __LINE__, ec, errno, fmt, __VA_ARGS__); return -1; } while (0)
#define RET_ERROR_UINT_FMT(ec,fmt,...)	do { push_error_stack_fmt_(__FILE__, __func__, __LINE__, ec, errno, fmt, __VA_ARGS__); return 0; } while (0)
#define RET_ERROR_PTR_FMT(ec,fmt,...)	do { push_error_stack_fmt_(__FILE__, __func__, __LINE__, ec, errno, fmt, __VA_ARGS__); return NULL; } while (0)
#define RET_ERROR_CUST_FMT(rv,ec,f,...)	do { push_error_stack_fmt_(__FILE__, __func__, __LINE__, ec, errno, f, __VA_ARGS__); return rv; } while (0)

#define PUSH_ERROR(ec,ax)		do { push_error_stack_(__FILE__, __func__, __LINE__, ec, errno, ax); } while (0)
#define PUSH_ERROR_FMT(ec,fmt,...)	do { push_error_stack_fmt_(__FILE__, __func__, __LINE__, ec, errno, fmt, __VA_ARGS__); } while (0)

#define PUSH_ERROR_SYSCALL(func)	do { push_error_stack_syscall_(__FILE__, __func__, __LINE__, errno, func ); } while (0)
#define PUSH_ERROR_OPENSSL()		do { push_error_stack_openssl_(__FILE__, __func__, __LINE__, ERR_OPENSSL, errno ); } while (0)
#define PUSH_ERROR_RESOLVER(func)	do { push_error_stack_resolver_(__FILE__, __func__, __LINE__, errno, h_errno, func ); } while (0)

#define PUBLIC_FUNC_PROLOGUE		{ clear_error_stack_(); }

#define PUBLIC_FUNC_IMPL(x, ...)		PUBLIC_FUNC_PROLOGUE; return(x ## _(__VA_ARGS__))
#define PUBLIC_FUNC_IMPL_VA1(x,p1)		PUBLIC_FUNC_PROLOGUE; { va_list ap; va_start(ap, p1); x ## __(p1, ap); va_end(ap); return; }
#define PUBLIC_FUNC_IMPL_VA1_RET(ret, x,p1)	PUBLIC_FUNC_PROLOGUE; { va_list ap; ret result; va_start(ap, p1); result = x ## __(p1, ap); va_end(ap); return result; }
#define PUBLIC_FUNC_IMPL_VA2(x,p1,p2, ...)	PUBLIC_FUNC_PROLOGUE; { va_list ap; va_start(ap, p2); x ## __(p1, p2, ap); va_end(ap); return; }
#define PUBLIC_FUNC_IMPL_VA2_RET(ret, x,p1,p2)	PUBLIC_FUNC_PROLOGUE; { va_list ap; ret result; va_start(ap, p2); result =  x ## __(p1, p2, ap); va_end(ap); return result; }

#define PUBLIC_FUNC_DECL(ret, x, ...)		ret x(__VA_ARGS__); \
						ret x ## _(__VA_ARGS__)

#define PUBLIC_FUNC_DECL_VA(ret, x, ...)	ret x(__VA_ARGS__, ...); \
						ret x ## _(__VA_ARGS__, ...); \
						ret x ## __(__VA_ARGS__, va_list ap);


#define ERR_SYSCALL		1
#define ERR_OPENSSL		2
#define ERR_RESOLVER		3
#define ERR_UNSPEC		4
#define ERR_BAD_PARAM		5
#define ERR_NOMEM		6
#define ERR_PERM		7


#define ERR_STACK_SIZE		8


typedef struct {
	unsigned int errcode;
	char *errmsg;
} err_desc_t;


struct errinfo {
	char filename[56];
	char funcname[56];
	int lineno;
	unsigned int errcode;
	int xerrno;
	char auxmsg[384];
} __attribute__ ((__packed__)); 

typedef struct errinfo errinfo_t;


// Public library routines.
const char *      get_error_string(unsigned int errcode);
const errinfo_t * get_last_error(void);
unsigned int      get_last_error_code(void);
errinfo_t *       pop_last_error(void);
const errinfo_t * get_first_error(void);
void              dump_last_error(void);
void              dump_error_stack(void);


// Internal error handling functions.
void              clear_error_stack_(void);
errinfo_t *       push_error_stack_(const char *filename, const char *funcname, int lineno, unsigned int errcode, int xerrno, char *auxmsg);
errinfo_t *       push_error_stack_fmt_(const char *filename, const char *funcname, int lineno, unsigned int errcode, int xerrno, const char *fmt, ...);
errinfo_t *       push_error_stack_syscall_(const char *filename, const char *funcname, int lineno, int xerrno, const char *errfunc);
errinfo_t *       push_error_stack_openssl_(const char *filename, const char *funcname, int lineno, unsigned int errcode, int xerrno);
errinfo_t *       push_error_stack_resolver_(const char *filename, const char *funcname, int lineno, int xerrno, int herrno, const char *errfunc);
errinfo_t *       create_new_error_(errinfo_t *errptr, const char *filename, const char *funcname, int lineno, unsigned int errcode, int xerrno, char *auxmsg);
void              dump_error_(const errinfo_t *error);

#endif
