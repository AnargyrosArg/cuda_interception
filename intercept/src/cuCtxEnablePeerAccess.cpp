#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxEnablePeerAccess)(
CUcontext, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxEnablePeerAccess(CUcontext peerContext, unsigned int Flags) {
		fprintf(stderr, "cuCtxEnablePeerAccess()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxEnablePeerAccess)
		{
			//fetch the original function addr using dlsym
			original_cuCtxEnablePeerAccess = (CUresult (*)(
			CUcontext, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuCtxEnablePeerAccess");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxEnablePeerAccess():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuCtxEnablePeerAccess(
		peerContext, 
		Flags
		);
	}
}