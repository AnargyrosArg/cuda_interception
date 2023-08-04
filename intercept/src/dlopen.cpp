#include <dlfcn.h>
#include <iostream>
#include <cstring>
#include <unistd.h>

//pointer for original dlopen
void* (*original_dlopen)(const char *filename, int flag);

//handle for the actual cuda driver library to be used by dlsym
void* original_libcuda_handle = NULL;
//handle for nvrtc lib
void* original_libnvrtc_handle = NULL;

void* libintercept_handle = NULL;




extern "C"{
    void* __libc_dlopen_mode (const char*,int); 


    void *dlopen(const char *filename, int flag){
        char* error;
        void* retval;

        fprintf(stderr,"dlopen intercepted filename: %s and flag: %d\n",filename,flag);
        //this call potentially clears previous errors that might have occured with dlopen,dlsym or dlclose
        dlerror();

        //get pointer to original dlopen function if not already fetched
        if(!original_dlopen){
    	    original_dlopen = (void* (*)(const char*,int)) dlsym(__libc_dlopen_mode("libdl.so.2",RTLD_LAZY), "dlopen");
            error = dlerror();
            if(error){
                fprintf(stderr,"Error occured while dlsym-ing dlopen: %s\n original_dlopen:%p\n",error,original_dlopen);
                exit(-1);
            }
        }
        if(libintercept_handle == NULL){
            libintercept_handle = original_dlopen("libintercept.so",RTLD_NOW | RTLD_GLOBAL);
            error = dlerror();
            if(error){
                fprintf(stderr,"Cannot find intercept library: %s\n",error);
               // fprintf(stderr,"LD_LIBRARY_PATH= %s\n",getenv("LD_LIBRARY_PATH"));
                exit(-1);
            }
        }

        //detect when a library attempts to load libcuda and instead, return our own.
        if(filename && (strcmp(filename,"libcuda.so.1")==0)){
            //if original libcuda has never been loaded before, we open it and keep a handle to it for internal use
            //this handle is not returned to the calling function
            if(original_libcuda_handle == NULL){
                original_libcuda_handle = original_dlopen("libcuda.so.1", RTLD_LAZY | RTLD_LOCAL);
                fprintf(stderr,"Interceptor just dlopened libcuda for the first time\n");
                // fprintf(stderr,"LOCAL | LAZY -> %d\n",RTLD_LAZY | RTLD_LOCAL);
                // fprintf(stderr,"GLOBAL | LAZY -> %d\n",RTLD_LAZY | RTLD_GLOBAL);
                // fprintf(stderr,"LOCAL | NOW -> %d\n",RTLD_NOW | RTLD_LOCAL);
                // fprintf(stderr,"GLOBAL | NOW -> %d\n",RTLD_NOW | RTLD_GLOBAL);
                fprintf(stderr,"original libcuda handle:%p\n",original_libcuda_handle);
            }
            //return our own lib instead
            retval = libintercept_handle;
            error = dlerror();
            if(error){
                fprintf(stderr,"%s\n",error);
                exit(-1);
            }

        }else if(filename && (strcmp(filename,"libnvrtc.so.11.2")==0)){
            //load libnvrtc for the first time
            if(original_libnvrtc_handle == NULL){
                original_libnvrtc_handle = original_dlopen(filename, flag);
                error = dlerror();
                if(error) fprintf(stderr,"dlopen libnvrtc.so.11.2: %s\n",error);
            }
            //return our own lib instead
            retval = libintercept_handle;
            
        }else{
            retval =  original_dlopen(filename,flag);
        }
        
        //its important not to exit even after getting an error as a lot of programs use try/except blocks
        //to check if some library can be found,and resume normal execution 
        error = dlerror();
        if(error) fprintf(stderr,"Error occured dlopen: %s\n retval:%p\n",error,retval);
    
        fprintf(stderr,"handle returned for %s -> %p\n",filename,retval);
        return retval;
    }
}
