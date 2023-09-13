#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamBeginCapture)(
cudaStream_t, 
enum cudaStreamCaptureMode
);
extern "C"
{
	cudaError_t cudaStreamBeginCapture(cudaStream_t stream, enum cudaStreamCaptureMode mode) {
		fprintf(stderr, "===============\ncudaStreamBeginCapture()\n");
		char* __dlerror;
		if (!original_cudaStreamBeginCapture)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamBeginCapture = (cudaError_t (*)(
			cudaStream_t, 
			enum cudaStreamCaptureMode)
			) dlsym(RTLD_NEXT, "cudaStreamBeginCapture");
			fprintf(stderr, "original_cudaStreamBeginCapture:%p\n", original_cudaStreamBeginCapture);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamBeginCapture():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamBeginCapture(
		stream, 
		mode
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}