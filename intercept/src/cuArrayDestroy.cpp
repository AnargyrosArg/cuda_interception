#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuArrayDestroy)(
CUarray
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuArrayDestroy(CUarray hArray) {
		fprintf(stderr, "===============\ncuArrayDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuArrayDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuArrayDestroy = (CUresult (*)(
			CUarray)
			) dlsym(original_libcuda_handle, "cuArrayDestroy");
			fprintf(stderr, "original_cuArrayDestroy:%p\n", original_cuArrayDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuArrayDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuArrayDestroy(
		hArray
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}