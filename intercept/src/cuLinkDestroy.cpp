#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuLinkDestroy)(
CUlinkState
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuLinkDestroy(CUlinkState state) {
		fprintf(stderr, "cuLinkDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuLinkDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuLinkDestroy = (CUresult (*)(
			CUlinkState)
			) dlsym(original_libcuda_handle, "cuLinkDestroy");
			fprintf(stderr, "original_cuLinkDestroy:%p\n", original_cuLinkDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuLinkDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuLinkDestroy(
		state
		);
	}
}