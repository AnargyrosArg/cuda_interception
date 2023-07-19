#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamAddCallback)(
CUstream, 
CUstreamCallback, 
void *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamAddCallback(CUstream hStream, CUstreamCallback callback, void *userData, unsigned int flags) {
		fprintf(stderr, "cuStreamAddCallback()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamAddCallback)
		{
			//fetch the original function addr using dlsym
			original_cuStreamAddCallback = (CUresult (*)(
			CUstream, 
			CUstreamCallback, 
			void *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamAddCallback");
			fprintf(stderr, "original_cuStreamAddCallback:%p\n", original_cuStreamAddCallback);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamAddCallback():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamAddCallback(
		hStream, 
		callback, 
		userData, 
		flags
		);
	}
}