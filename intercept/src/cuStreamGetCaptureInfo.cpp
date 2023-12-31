#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamGetCaptureInfo)(
CUstream, 
CUstreamCaptureStatus *, 
cuuint64_t *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamGetCaptureInfo(CUstream hStream, CUstreamCaptureStatus *captureStatus_out, cuuint64_t *id_out) {
		fprintf(stderr, "===============\ncuStreamGetCaptureInfo()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamGetCaptureInfo)
		{
			//fetch the original function addr using dlsym
			original_cuStreamGetCaptureInfo = (CUresult (*)(
			CUstream, 
			CUstreamCaptureStatus *, 
			cuuint64_t *)
			) dlsym(original_libcuda_handle, "cuStreamGetCaptureInfo");
			fprintf(stderr, "original_cuStreamGetCaptureInfo:%p\n", original_cuStreamGetCaptureInfo);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamGetCaptureInfo():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamGetCaptureInfo(
		hStream, 
		captureStatus_out, 
		id_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}