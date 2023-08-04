#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxGetCurrent)(
CUcontext *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxGetCurrent(CUcontext *pctx) {
		fprintf(stderr, "===============\ncuCtxGetCurrent()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxGetCurrent)
		{
			//fetch the original function addr using dlsym
			original_cuCtxGetCurrent = (CUresult (*)(
			CUcontext *)
			) dlsym(original_libcuda_handle, "cuCtxGetCurrent");
			fprintf(stderr, "original_cuCtxGetCurrent:%p\n", original_cuCtxGetCurrent);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxGetCurrent():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuCtxGetCurrent(
		pctx
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}