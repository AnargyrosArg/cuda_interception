#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpy2DFromArrayAsync)(
void *, 
size_t, 
cudaArray_const_t, 
size_t, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpy2DFromArrayAsync(void *dst, size_t dpitch, cudaArray_const_t src, size_t wOffset, size_t hOffset, size_t width, size_t height, enum cudaMemcpyKind kind, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpy2DFromArrayAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpy2DFromArrayAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpy2DFromArrayAsync = (cudaError_t (*)(
			void *, 
			size_t, 
			cudaArray_const_t, 
			size_t, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpy2DFromArrayAsync");
			fprintf(stderr, "original_cudaMemcpy2DFromArrayAsync:%p\n", original_cudaMemcpy2DFromArrayAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpy2DFromArrayAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpy2DFromArrayAsync(
		dst, 
		dpitch, 
		src, 
		wOffset, 
		hOffset, 
		width, 
		height, 
		kind, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}