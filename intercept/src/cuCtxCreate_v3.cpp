#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxCreate_v3)(
CUcontext *, 
CUexecAffinityParam *, 
int, 
unsigned int, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxCreate_v3(CUcontext *pctx, CUexecAffinityParam *paramsArray, int numParams, unsigned int flags, CUdevice dev) {
		fprintf(stderr, "cuCtxCreate_v3()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxCreate_v3)
		{
			//fetch the original function addr using dlsym
			original_cuCtxCreate_v3 = (CUresult (*)(
			CUcontext *, 
			CUexecAffinityParam *, 
			int, 
			unsigned int, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuCtxCreate_v3");
			fprintf(stderr, "original_cuCtxCreate_v3:%p\n", original_cuCtxCreate_v3);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxCreate_v3():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxCreate_v3(
		pctx, 
		paramsArray, 
		numParams, 
		flags, 
		dev
		);
	}
}