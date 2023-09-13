#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyToArrayAsync)(
cudaArray_t, 
size_t, 
size_t, 
const void *, 
size_t, 
enum cudaMemcpyKind, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpyToArrayAsync(cudaArray_t dst, size_t wOffset, size_t hOffset, const void *src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpyToArrayAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpyToArrayAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyToArrayAsync = (cudaError_t (*)(
			cudaArray_t, 
			size_t, 
			size_t, 
			const void *, 
			size_t, 
			enum cudaMemcpyKind, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpyToArrayAsync");
			fprintf(stderr, "original_cudaMemcpyToArrayAsync:%p\n", original_cudaMemcpyToArrayAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyToArrayAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyToArrayAsync(
		dst, 
		wOffset, 
		hOffset, 
		src, 
		count, 
		kind, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}