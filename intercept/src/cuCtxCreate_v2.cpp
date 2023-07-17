#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxCreate_v2)(
CUcontext *, 
unsigned int, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxCreate_v2(CUcontext *pctx, unsigned int flags, CUdevice dev) {
		fprintf(stderr, "cuCtxCreate_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxCreate_v2)
		{
			//fetch the original function addr using dlsym
			original_cuCtxCreate_v2 = (CUresult (*)(
			CUcontext *, 
			unsigned int, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuCtxCreate_v2");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxCreate_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxCreate_v2(
		pctx, 
		flags, 
		dev
		);
	}
}