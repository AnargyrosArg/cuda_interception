#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyFromSymbolAsync)(
void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind, 
cudaStream_t
);
extern "C"
{
	cudaError_t cudaMemcpyFromSymbolAsync(void *dst, const void *symbol, size_t count, size_t offset, enum cudaMemcpyKind kind, cudaStream_t stream) {
		fprintf(stderr, "===============\ncudaMemcpyFromSymbolAsync()\n");
		char* __dlerror;
		if (!original_cudaMemcpyFromSymbolAsync)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyFromSymbolAsync = (cudaError_t (*)(
			void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind, 
			cudaStream_t)
			) dlsym(RTLD_NEXT, "cudaMemcpyFromSymbolAsync");
			fprintf(stderr, "original_cudaMemcpyFromSymbolAsync:%p\n", original_cudaMemcpyFromSymbolAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyFromSymbolAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyFromSymbolAsync(
		dst, 
		symbol, 
		count, 
		offset, 
		kind, 
		stream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}