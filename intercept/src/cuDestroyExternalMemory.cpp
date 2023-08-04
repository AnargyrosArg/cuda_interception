#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDestroyExternalMemory)(
CUexternalMemory
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDestroyExternalMemory(CUexternalMemory extMem) {
		fprintf(stderr, "===============\ncuDestroyExternalMemory()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDestroyExternalMemory)
		{
			//fetch the original function addr using dlsym
			original_cuDestroyExternalMemory = (CUresult (*)(
			CUexternalMemory)
			) dlsym(original_libcuda_handle, "cuDestroyExternalMemory");
			fprintf(stderr, "original_cuDestroyExternalMemory:%p\n", original_cuDestroyExternalMemory);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDestroyExternalMemory():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDestroyExternalMemory(
		extMem
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}