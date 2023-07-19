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

extern "C"{
    void *dlopen(const char *filename, int flag){
        char* error;
        void* retval;

        fprintf(stderr,"dlopen intercepted filename: %s and flag: %d\n",filename,flag);

        //this call potentially clears previous errors that might have occured with dlopen,dlsym or dlclose
        dlerror();

        //get pointer to original dlopen function if not already fetched
        if(!original_dlopen){
    	    original_dlopen = (void* (*)(const char*,int)) dlsym(RTLD_NEXT, "dlopen");
            error = dlerror();
            if(error){
                fprintf(stderr,"Error occured while dlsym-ing dlopen: %s\n original_dlopen:%p\n",error,original_dlopen);
                exit(-1);
            }
        }

        //detect when a library attempts to load libcuda and instead, return our own.
        if(filename && (strcmp(filename,"libcuda.so.1")==0)){
            //if original libcuda has never been loaded before, we open it and keep a handle to it for internal use
            //this handle is not returned to the calling function
            if(original_libcuda_handle == NULL){
                original_libcuda_handle = original_dlopen("libcuda.so.1", flag);
            }
            //return our own lib instead
            retval = original_dlopen("libintercept.so", RTLD_NOW | RTLD_GLOBAL);
            error = dlerror();
            if(error){
                fprintf(stderr,"Cannot find intercept library: %s\n",error);
                exit(-1);
            }

        }else if(filename && (strcmp(filename,"libnvrtc.so.11.2")==0)){
            //load libnvrtc for the first time
            if(original_libnvrtc_handle == NULL){
                original_libnvrtc_handle = original_dlopen("libnvrtc.so.11.2", flag);
                error = dlerror();
                if(error) fprintf(stderr,"dlopen libnvrtc: %s\n",error);
            }
            //return our own lib instead
            retval = original_dlopen("libintercept.so",flag);
            error = dlerror();
            if(error){
                fprintf(stderr,"Cannot find intercept library: %s\n",error);
               // fprintf(stderr,"LD_LIBRARY_PATH= %s\n",getenv("LD_LIBRARY_PATH"));
                exit(-1);
            }
        }else{
            retval =  original_dlopen(filename,flag);
        }
        
        //its important not to exit even after getting an error as a lot of programs use try/except blocks
        //to check if some library can be found,and resume normal execution 
        error = dlerror();
        if(error) fprintf(stderr,"Error occured dlopen: %s\n retval:%p\n",error,retval);
    
        //fprintf(stderr,">%s< original handle for libcuda: %p -- actually returning %p\n",filename,original_libcuda_handle,retval);
        return retval;
    }
}
