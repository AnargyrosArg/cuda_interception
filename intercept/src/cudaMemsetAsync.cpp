#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemsetAsync)(
void *, 
int, 
size_t, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemsetAsync(void *devPtr, int value, size_t count, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemsetAsync()\n");
		char* __dlerror;
		if (!original_cudaMemsetAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemsetAsync = (cudaError_t (*)(
			void *, 
			int, 
			size_t, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemsetAsync");
			fprintf(stderr, "original_cudaMemsetAsync:%p\n", original_cudaMemsetAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemsetAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemsetAsync(
		devPtr, 
		value, 
		count, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}