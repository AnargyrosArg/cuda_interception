#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy2DToArrayAsync)(
cudaArray_t, 
size_t, 
size_t, 
const void *, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpy2DToArrayAsync(cudaArray_t dst, size_t wOffset, size_t hOffset, const void *src, size_t spitch, size_t width, size_t height, enum cudaMemcpyKind kind, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpy2DToArrayAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpy2DToArrayAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy2DToArrayAsync = (cudaError_t (*)(
			cudaArray_t, 
			size_t, 
			size_t, 
			const void *, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpy2DToArrayAsync");
			fprintf(stderr, "original_cudaMemcpy2DToArrayAsync:%p\n", original_cudaMemcpy2DToArrayAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy2DToArrayAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy2DToArrayAsync(
		dst, 
		wOffset, 
		hOffset, 
		src, 
		spitch, 
		width, 
		height, 
		kind, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}