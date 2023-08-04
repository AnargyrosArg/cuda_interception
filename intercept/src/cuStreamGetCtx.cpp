#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamGetCtx)(
CUstream, 
CUcontext *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamGetCtx(CUstream hStream, CUcontext *pctx) {
		fprintf(stderr, "===============\ncuStreamGetCtx()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamGetCtx)
		{
			//fetch the original function addr using dlsym
			original_cuStreamGetCtx = (CUresult (*)(
			CUstream, 
			CUcontext *)
			) dlsym(original_libcuda_handle, "cuStreamGetCtx");
			fprintf(stderr, "original_cuStreamGetCtx:%p\n", original_cuStreamGetCtx);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamGetCtx():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamGetCtx(
		hStream, 
		pctx
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}