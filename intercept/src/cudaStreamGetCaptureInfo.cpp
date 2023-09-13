#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamGetCaptureInfo)(
cudaStream_t, 
enum cudaStreamCaptureStatus *, 
unsigned long long *
);
extern "C"
{
	cudaError_t cudaStreamGetCaptureInfo(cudaStream_t stream, enum cudaStreamCaptureStatus *pCaptureStatus, unsigned long long *pId) {
		fprintf(stderr, "===============\ncudaStreamGetCaptureInfo()\n");
		char* __dlerror;
		if (!original_cudaStreamGetCaptureInfo)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamGetCaptureInfo = (cudaError_t (*)(
			cudaStream_t, 
			enum cudaStreamCaptureStatus *, 
			unsigned long long *)
			) dlsym(RTLD_NEXT, "cudaStreamGetCaptureInfo");
			fprintf(stderr, "original_cudaStreamGetCaptureInfo:%p\n", original_cudaStreamGetCaptureInfo);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamGetCaptureInfo():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamGetCaptureInfo(
		stream, 
		pCaptureStatus, 
		pId
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}