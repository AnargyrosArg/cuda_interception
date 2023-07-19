#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamIsCapturing)(
CUstream, 
CUstreamCaptureStatus *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamIsCapturing(CUstream hStream, CUstreamCaptureStatus *captureStatus) {
		fprintf(stderr, "cuStreamIsCapturing()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamIsCapturing)
		{
			//fetch the original function addr using dlsym
			original_cuStreamIsCapturing = (CUresult (*)(
			CUstream, 
			CUstreamCaptureStatus *)
			) dlsym(original_libcuda_handle, "cuStreamIsCapturing");
			fprintf(stderr, "original_cuStreamIsCapturing:%p\n", original_cuStreamIsCapturing);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamIsCapturing():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamIsCapturing(
		hStream, 
		captureStatus
		);
	}
}