#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyToSymbolAsync)(
const void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpyToSymbolAsync(const void *symbol, const void *src, size_t count, size_t offset, enum cudaMemcpyKind kind, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpyToSymbolAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpyToSymbolAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyToSymbolAsync = (cudaError_t (*)(
			const void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpyToSymbolAsync");
			fprintf(stderr, "original_cudaMemcpyToSymbolAsync:%p\n", original_cudaMemcpyToSymbolAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyToSymbolAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyToSymbolAsync(
		symbol, 
		src, 
		count, 
		offset, 
		kind, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}