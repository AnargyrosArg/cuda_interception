#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyAsync)(
void* dst,
const void *src,
size_t, 
cudaMemcpyKind, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpyAsync(void *dst, const void *src, size_t count, cudaMemcpyKind kind, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpyAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpyAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyAsync = (cudaError_t (*)(
			void*,
			const void*,
			size_t, 
			enum cudaMemcpyKind, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpyAsync");
			fprintf(stderr, "original_cudaMemcpyAsync:%p\n", original_cudaMemcpyAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyAsync(
		dst,
		src,
		count, 
		kind, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}