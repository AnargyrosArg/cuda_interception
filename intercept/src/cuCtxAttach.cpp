#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxAttach)(
CUcontext *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxAttach(CUcontext *pctx, unsigned int flags) {
		fprintf(stderr, "cuCtxAttach()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxAttach)
		{
			//fetch the original function addr using dlsym
			original_cuCtxAttach = (CUresult (*)(
			CUcontext *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuCtxAttach");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxAttach():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxAttach(
		pctx, 
		flags
		);
	}
}