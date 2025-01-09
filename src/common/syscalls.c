#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#undef errno
extern int errno;

int _getpid(void)
{
  return 1;
}

int _kill(int pid, int sig)
{
  errno = EINVAL;
  return -1;
}

void _exit(int status)
{
  while (1)
  {
    // TODO: Change types.h to be includable from c file.
    __asm__("BKPT #0"); // debug break point
  }
}

int _write(int file, char *ptr, int len)
{
  for (int i = 0; i < len; i++)
  {
    // TODO: UART write
  }
  return len; // Returns number of characters written
}

int _read(int file, char *ptr, int len)
{
  // TODO: UART read
  errno = EINVAL;
  return -1;
}

int _fstat(int file, struct stat *st)
{
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
  // 1 means it is a terminal device
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
  return 0;
}

void *_sbrk(ptrdiff_t increment)
{
  // dynamic allocation is not enabled
  (void)increment;
  errno = ENOMEM;
  return (void *)-1;
}