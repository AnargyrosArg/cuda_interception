#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxDestroy_v2)(
CUcontext
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxDestroy_v2(CUcontext ctx) {
		fprintf(stderr, "cuCtxDestroy_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxDestroy_v2)
		{
			//fetch the original function addr using dlsym
			original_cuCtxDestroy_v2 = (CUresult (*)(
			CUcontext)
			) dlsym(original_libcuda_handle, "cuCtxDestroy_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxDestroy_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxDestroy_v2(
		ctx
		);
	}
}