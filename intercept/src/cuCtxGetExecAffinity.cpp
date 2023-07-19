#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetExecAffinity)(
CUexecAffinityParam *, 
CUexecAffinityType
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetExecAffinity(CUexecAffinityParam *pExecAffinity, CUexecAffinityType type) {
		fprintf(stderr, "cuCtxGetExecAffinity()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetExecAffinity)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetExecAffinity = (CUresult (*)(
			CUexecAffinityParam *, 
			CUexecAffinityType)
			) dlsym(original_libcuda_handle, "cuCtxGetExecAffinity");
			fprintf(stderr, "original_cuCtxGetExecAffinity:%p\n", original_cuCtxGetExecAffinity);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetExecAffinity():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxGetExecAffinity(
		pExecAffinity, 
		type
		);
	}
}