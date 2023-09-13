#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyFromArrayAsync)(
void *, 
cudaArray_const_t, 
size_t, 
size_t, 
size_t, 
enum cudaMemcpyKind, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpyFromArrayAsync(void *dst, cudaArray_const_t src, size_t wOffset, size_t hOffset, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpyFromArrayAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpyFromArrayAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyFromArrayAsync = (cudaError_t (*)(
			void *, 
			cudaArray_const_t, 
			size_t, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpyFromArrayAsync");
			fprintf(stderr, "original_cudaMemcpyFromArrayAsync:%p\n", original_cudaMemcpyFromArrayAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyFromArrayAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyFromArrayAsync(
		dst, 
		src, 
		wOffset, 
		hOffset, 
		count, 
		kind, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}