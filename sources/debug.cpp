#ifdef _WIN32
#include "windows.h"
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "filehandling.h"
#include "wii_utils.h"

#ifdef KITSCHY_DEBUG_MEMORY
#include "debug_memorymanager.h"
#endif

static FILE *fp = NULL;

void output_debug_message(const char *fmt, ...)
{
#if 0
    char text[256], fname[256];    
    va_list ap;
    memset(text, 0, 256);
    memset(fname, 0, 256);  

    if (fmt == 0)
        return ;
        
    if (fp == NULL) {        
        fp = fopen("mog_debug.log", "a");
    }

    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);    

    fclose(fp);
    fp = NULL;
#endif
}

void close_debug_messages(void)
{
    fclose(fp);
    fp = 0;
} /* close_debug_messages */
