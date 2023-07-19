#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuSurfObjectDestroy)(
CUsurfObject
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuSurfObjectDestroy(CUsurfObject surfObject) {
		fprintf(stderr, "cuSurfObjectDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuSurfObjectDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuSurfObjectDestroy = (CUresult (*)(
			CUsurfObject)
			) dlsym(original_libcuda_handle, "cuSurfObjectDestroy");
			fprintf(stderr, "original_cuSurfObjectDestroy:%p\n", original_cuSurfObjectDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuSurfObjectDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuSurfObjectDestroy(
		surfObject
		);
	}
}