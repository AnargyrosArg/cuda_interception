#include <cudaEGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuEventCreateFromEGLSync)(
CUevent *, 
EGLSyncKHR, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuEventCreateFromEGLSync(CUevent *phEvent, EGLSyncKHR eglSync, unsigned int flags) {
		fprintf(stderr, "cuEventCreateFromEGLSync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuEventCreateFromEGLSync)
		{
			//fetch the original function addr using dlsym
			original_cuEventCreateFromEGLSync = (CUresult (*)(
			CUevent *, 
			EGLSyncKHR, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuEventCreateFromEGLSync");
			fprintf(stderr, "original_cuEventCreateFromEGLSync:%p\n", original_cuEventCreateFromEGLSync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuEventCreateFromEGLSync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuEventCreateFromEGLSync(
		phEvent, 
		eglSync, 
		flags
		);
	}
}