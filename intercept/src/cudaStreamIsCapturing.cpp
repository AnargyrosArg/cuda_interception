#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamIsCapturing)(
cudaStream_t, 
enum cudaStreamCaptureStatus *
);
extern "C"
{
	cudaError_t cudaStreamIsCapturing(cudaStream_t stream, enum cudaStreamCaptureStatus *pCaptureStatus) {
		fprintf(stderr, "===============\ncudaStreamIsCapturing()\n");
		char* __dlerror;
		if (!original_cudaStreamIsCapturing)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamIsCapturing = (cudaError_t (*)(
			cudaStream_t, 
			enum cudaStreamCaptureStatus *)
			) dlsym(RTLD_NEXT, "cudaStreamIsCapturing");
			fprintf(stderr, "original_cudaStreamIsCapturing:%p\n", original_cudaStreamIsCapturing);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamIsCapturing():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamIsCapturing(
		stream, 
		pCaptureStatus
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}