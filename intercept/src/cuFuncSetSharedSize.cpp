#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuFuncSetSharedSize)(
CUfunction, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuFuncSetSharedSize(CUfunction hfunc, unsigned int bytes) {
		fprintf(stderr, "===============\ncuFuncSetSharedSize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuFuncSetSharedSize)
		{
			//fetch the original function addr using dlsym
			original_cuFuncSetSharedSize = (CUresult (*)(
			CUfunction, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuFuncSetSharedSize");
			fprintf(stderr, "original_cuFuncSetSharedSize:%p\n", original_cuFuncSetSharedSize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuFuncSetSharedSize():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuFuncSetSharedSize(
		hfunc, 
		bytes
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}