#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamSynchronize)(
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamSynchronize(CUstream hStream) {
		fprintf(stderr, "cuStreamSynchronize()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamSynchronize)
		{
			//fetch the original function addr using dlsym
			original_cuStreamSynchronize = (CUresult (*)(
			CUstream)
			) dlsym(original_libcuda_handle, "cuStreamSynchronize");
			fprintf(stderr, "original_cuStreamSynchronize:%p\n", original_cuStreamSynchronize);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamSynchronize():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamSynchronize(
		hStream
		);
	}
}