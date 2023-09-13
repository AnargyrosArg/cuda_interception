#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaFreeAsync)(
void *, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaFreeAsync(void *devPtr, cudaStream_t hStream) {
		fprintf(stderr, "===============\ncudaFreeAsync()\n");
		char* __dlerror;
		if (!original_cudaFreeAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaFreeAsync = (cudaError_t (*)(
			void *, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaFreeAsync");
			fprintf(stderr, "original_cudaFreeAsync:%p\n", original_cudaFreeAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaFreeAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaFreeAsync(
		devPtr, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}