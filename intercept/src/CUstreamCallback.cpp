/*#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
void (*original_CUstreamCallback)(
CUstream, 
CUresult, 
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	// void CUstreamCallback(CUstream hStream, CUresult status, void *userData) {
		fprintf(stderr, "===============\nCUstreamCallback()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_CUstreamCallback)
		{
			//fetch the original function addr using dlsym
			original_CUstreamCallback = (void (*)(
			CUstream, 
			CUresult, 
			void *)
			) dlsym(original_libcuda_handle, "CUstreamCallback");
			fprintf(stderr, "original_CUstreamCallback:%p\n", original_CUstreamCallback);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function CUstreamCallback():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_CUstreamCallback(
		hStream, 
		status, 
		userData
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}
*/
