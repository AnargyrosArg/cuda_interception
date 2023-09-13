#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamAddCallback)(
cudaStream_t, 
cudaStreamCallback_t, 
void *, 
unsigned int
);
extern "C"
{
	cudaError_t cudaStreamAddCallback(cudaStream_t stream, cudaStreamCallback_t callback, void *userData, unsigned int flags) {
		fprintf(stderr, "===============\ncudaStreamAddCallback()\n");
		char* __dlerror;
		if (!original_cudaStreamAddCallback)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamAddCallback = (cudaError_t (*)(
			cudaStream_t, 
			cudaStreamCallback_t, 
			void *, 
			unsigned int)
			) dlsym(RTLD_NEXT, "cudaStreamAddCallback");
			fprintf(stderr, "original_cudaStreamAddCallback:%p\n", original_cudaStreamAddCallback);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamAddCallback():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamAddCallback(
		stream, 
		callback, 
		userData, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}