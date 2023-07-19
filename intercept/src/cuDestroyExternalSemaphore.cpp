#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDestroyExternalSemaphore)(
CUexternalSemaphore
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDestroyExternalSemaphore(CUexternalSemaphore extSem) {
		fprintf(stderr, "cuDestroyExternalSemaphore()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDestroyExternalSemaphore)
		{
			//fetch the original function addr using dlsym
			original_cuDestroyExternalSemaphore = (CUresult (*)(
			CUexternalSemaphore)
			) dlsym(original_libcuda_handle, "cuDestroyExternalSemaphore");
			fprintf(stderr, "original_cuDestroyExternalSemaphore:%p\n", original_cuDestroyExternalSemaphore);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDestroyExternalSemaphore():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDestroyExternalSemaphore(
		extSem
		);
	}
}