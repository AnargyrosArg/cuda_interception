#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemPrefetchAsync)(
const void *, 
size_t, 
int, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemPrefetchAsync(const void *devPtr, size_t count, int dstDevice, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemPrefetchAsync()\n");
		char* __dlerror;
		if (!original_cudaMemPrefetchAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemPrefetchAsync = (cudaError_t (*)(
			const void *, 
			size_t, 
			int, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemPrefetchAsync");
			fprintf(stderr, "original_cudaMemPrefetchAsync:%p\n", original_cudaMemPrefetchAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemPrefetchAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemPrefetchAsync(
		devPtr, 
		count, 
		dstDevice, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}