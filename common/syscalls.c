/*
** syscalls.c
*/

#include <stdint.h>
#include <stdlib.h>
#include <reent.h>
#include <sys/stat.h>

#include <errno.h>
#undef errno
extern int errno;

#include <board.h>
#include <dbgu.h>
#include <sdram.h>

_ssize_t _read_r(struct _reent *r, int file, void *ptr, size_t len)
{
	return dbgu_getc((char*)ptr);
}

_ssize_t _write_r(struct _reent *r, int file, const void *ptr, size_t len)
{
	size_t todo = len;
	const unsigned char *p = ptr;

	while(todo) {	
		dbgu_putc(*p++);
		todo--;
    }
	
	return (_ssize_t)len;
}

int _close_r(struct _reent *r, int file)
{
	return 0;
}

_off_t _lseek_r(struct _reent *r, int file, _off_t ptr, int dir)
{
	return (_off_t)0;
}

int _fstat_r(struct _reent *r, int file, struct stat *st)
{
	/* character device */
	st->st_mode = S_IFCHR;
	return 0;
}

int isatty(int file)
{
	return 1;
}

int _isatty(int file)
{
	return 1;
}
/*
void __fixunsdfsi(void)
{

}
*/
#if 0
/* we use a simpler method, see csetup.S near branching to main */
static void _exit (int n) {
label:  goto label; /* endless loop */
}
#endif

static uint8_t *heap_ptr; /* current end of heap (.bss-section -> always zero after reset) */
void *_sbrk_r(struct _reent *_s_r, ptrdiff_t nbytes)
{
	/* start heap at the beginning of our sdram */
	if(!heap_ptr) {
		heap_ptr = (uint8_t*)SDRAM_BASE;
	}
	uint8_t *ptr = heap_ptr;
	
	/* out of memory? */
	if((uint32_t)ptr + nbytes >= (SDRAM_BASE+HEAP_SIZE)) { /* >? */
		errno = ENOMEM;
		return (void*)0;
	}

	/* move end of heap */
	heap_ptr += nbytes;
	
	/* return pointer to allocated memory */
	return ptr;
}

size_t heap_usage(void)
{
	if(!heap_ptr) return 0;
	return (unsigned int)heap_ptr - SDRAM_BASE;
}

int _kill(int pid, int sig)
{
//    pid = pid; sig = sig; /* avoid warnings */
    errno = EINVAL;
    return -1;
}

void _exit(int status)
{
//    xprintf("_exit called with parameter %d\n", status);
    while(1) {;}
}

int _getpid(void)
{
    return 1;
}

void __exidx_end (int status)
{
//    xprintf("_exit called with parameter %d\n", status);
     {;}
}

void __exidx_start (int status)
{
//    xprintf("_exit called with parameter %d\n", status);
     {;}
}

