#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMallocAsync)(
void **, 
size_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMallocAsync(void **devPtr, size_t size, cudaStream_t hStream) {
		fprintf(stderr, "===============\ncudaMallocAsync()\n");
		char* __dlerror;
		if (!original_cudaMallocAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMallocAsync = (cudaError_t (*)(
			void **, 
			size_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMallocAsync");
			fprintf(stderr, "original_cudaMallocAsync:%p\n", original_cudaMallocAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMallocAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMallocAsync(
		devPtr, 
		size, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}