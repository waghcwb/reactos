/* $Id: read.c,v 1.8 2002/09/08 10:22:50 chorns Exp $
 *
 * COPYRIGHT:   See COPYING in the top level directory
 * PROJECT:     ReactOS system libraries
 * FILE:        lib/msvcrt/io/read.c
 * PURPOSE:     Reads a file
 * PROGRAMER:   Boudewijn Dekker
 * UPDATE HISTORY:
 *              28/12/1998: Created
 *              03/05/2002: made _read() non-greedy - it now returns as soon as
 *                          any amount of data has been read. It's the expected
 *                          behavior for line-buffered streams (KJK::Hyperion)
 */
#include <windows.h>
#include <msvcrt/io.h>
#include <msvcrt/internal/file.h>

#define NDEBUG
#include <msvcrt/msvcrtdbg.h>

size_t _read(int _fd, void *_buf, size_t _nbyte)
{
   DWORD _rbyte = 0, nbyte = _nbyte;
   char *bufp = (char*)_buf;
   HANDLE hfile;
   int istext, error;

   DPRINT("_read(fd %d, buf %x, nbyte %d)\n", _fd, _buf, _nbyte);

   /* null read */
   if(_nbyte == 0)
      return 0;

   hfile = _get_osfhandle(_fd);
   istext = __fileno_getmode(_fd) & O_TEXT;

   /* read data */
   if (!ReadFile(hfile, bufp, nbyte, &_rbyte, NULL))
   {
      /* failure */
      error = GetLastError();
      if (error == ERROR_BROKEN_PIPE)
      {
	 return 0;
      }
      return -1;
   }
      
   /* text mode */
   if (_rbyte && istext)
   {
      int cr = 0;
      DWORD count = _rbyte;

      /* repeat for all bytes in the buffer */
      for(; count; bufp++, count--)
      {
         /* carriage return */
         if (*bufp == '\r')
            cr++;
         /* shift characters back, to ignore carriage returns */
         else if (cr != 0)
            *(bufp - cr) = *bufp;

      }

      /* ignore the carriage returns */
      _rbyte -= cr;
   }

   DPRINT("%d\n", _rbyte);
   return _rbyte;
}
