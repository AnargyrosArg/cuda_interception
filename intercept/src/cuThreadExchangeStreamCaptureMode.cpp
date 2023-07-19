#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuThreadExchangeStreamCaptureMode)(
CUstreamCaptureMode *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuThreadExchangeStreamCaptureMode(CUstreamCaptureMode *mode) {
		fprintf(stderr, "cuThreadExchangeStreamCaptureMode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuThreadExchangeStreamCaptureMode)
		{
			//fetch the original function addr using dlsym
			original_cuThreadExchangeStreamCaptureMode = (CUresult (*)(
			CUstreamCaptureMode *)
			) dlsym(original_libcuda_handle, "cuThreadExchangeStreamCaptureMode");
			fprintf(stderr, "original_cuThreadExchangeStreamCaptureMode:%p\n", original_cuThreadExchangeStreamCaptureMode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuThreadExchangeStreamCaptureMode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuThreadExchangeStreamCaptureMode(
		mode
		);
	}
}