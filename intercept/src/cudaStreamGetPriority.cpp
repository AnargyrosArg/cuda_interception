#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamGetPriority)(
cudaStream_t, 
int *
);
extern "C"
{
	cudaError_t cudaStreamGetPriority(cudaStream_t hStream, int *priority) {
		fprintf(stderr, "===============\ncudaStreamGetPriority()\n");
		char* __dlerror;
		if (!original_cudaStreamGetPriority)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamGetPriority = (cudaError_t (*)(
			cudaStream_t, 
			int *)
			) dlsym(RTLD_NEXT, "cudaStreamGetPriority");
			fprintf(stderr, "original_cudaStreamGetPriority:%p\n", original_cudaStreamGetPriority);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamGetPriority():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamGetPriority(
		hStream, 
		priority
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}