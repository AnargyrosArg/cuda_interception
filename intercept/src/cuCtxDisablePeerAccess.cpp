#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxDisablePeerAccess)(
CUcontext
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxDisablePeerAccess(CUcontext peerContext) {
		fprintf(stderr, "cuCtxDisablePeerAccess()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxDisablePeerAccess)
		{
			//fetch the original function addr using dlsym
			original_cuCtxDisablePeerAccess = (CUresult (*)(
			CUcontext)
			) dlsym(original_libcuda_handle, "cuCtxDisablePeerAccess");
			fprintf(stderr, "original_cuCtxDisablePeerAccess:%p\n", original_cuCtxDisablePeerAccess);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxDisablePeerAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxDisablePeerAccess(
		peerContext
		);
	}
}