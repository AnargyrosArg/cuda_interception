#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuCtxDetach)(
CUcontext
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuCtxDetach(CUcontext ctx) {
		fprintf(stderr, "===============\ncuCtxDetach()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuCtxDetach)
		{
			//fetch the original function addr using dlsym
			original_cuCtxDetach = (CUresult (*)(
			CUcontext)
			) dlsym(original_libcuda_handle, "cuCtxDetach");
			fprintf(stderr, "original_cuCtxDetach:%p\n", original_cuCtxDetach);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuCtxDetach():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuCtxDetach(
		ctx
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}