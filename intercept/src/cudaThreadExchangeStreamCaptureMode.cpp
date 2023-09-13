#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaThreadExchangeStreamCaptureMode)(
enum cudaStreamCaptureMode *
);
extern "C"
{
	cudaError_t cudaThreadExchangeStreamCaptureMode(enum cudaStreamCaptureMode *mode) {
		fprintf(stderr, "===============\ncudaThreadExchangeStreamCaptureMode()\n");
		char* __dlerror;
		if (!original_cudaThreadExchangeStreamCaptureMode)
		{
			//fetch the original function addr using dlsym
			original_cudaThreadExchangeStreamCaptureMode = (cudaError_t (*)(
			enum cudaStreamCaptureMode *)
			) dlsym(RTLD_NEXT, "cudaThreadExchangeStreamCaptureMode");
			fprintf(stderr, "original_cudaThreadExchangeStreamCaptureMode:%p\n", original_cudaThreadExchangeStreamCaptureMode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaThreadExchangeStreamCaptureMode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaThreadExchangeStreamCaptureMode(
		mode
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}