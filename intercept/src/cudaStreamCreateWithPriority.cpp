#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaStreamCreateWithPriority)(
cudaStream_t *, 
unsigned int, 
int
);
extern "C"
{
	cudaError_t cudaStreamCreateWithPriority(cudaStream_t *pStream, unsigned int flags, int priority) {
		fprintf(stderr, "===============\ncudaStreamCreateWithPriority()\n");
		char* __dlerror;
		if (!original_cudaStreamCreateWithPriority)
		{
			//fetch the original function addr using dlsym
			original_cudaStreamCreateWithPriority = (cudaError_t (*)(
			cudaStream_t *, 
			unsigned int, 
			int)
			) dlsym(RTLD_NEXT, "cudaStreamCreateWithPriority");
			fprintf(stderr, "original_cudaStreamCreateWithPriority:%p\n", original_cudaStreamCreateWithPriority);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaStreamCreateWithPriority():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaStreamCreateWithPriority(
		pStream, 
		flags, 
		priority
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}