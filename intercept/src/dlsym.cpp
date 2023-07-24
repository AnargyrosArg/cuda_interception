#include <dlfcn.h>
#include <iostream>
#include <unistd.h>
#include <cstring>


extern "C" { 
    void* __libc_dlsym (void *map, const char *name);
    void* __libc_dlopen_mode (const char*,int); 
}

void* (*original_dlsym)(void*,const char*) = NULL;


void* dlsym(void *handle, const char *symbol)
{
    fprintf(stderr,"dlsym called for symbol %s and handle %p\n",symbol,handle);
    if(original_dlsym == NULL){
        original_dlsym = (void* (*)(void*, const char*)) __libc_dlsym(__libc_dlopen_mode("libdl.so.2",RTLD_LAZY), "dlsym");
    }
    return (original_dlsym(handle, symbol));
}

