#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyToSymbol)(
const void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpyToSymbol(const void *symbol, const void *src, size_t count, size_t offset, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpyToSymbol()\n");
		char* __dlerror;
		if (!original_cudaMemcpyToSymbol)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyToSymbol = (cudaError_t (*)(
			const void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpyToSymbol");
			fprintf(stderr, "original_cudaMemcpyToSymbol:%p\n", original_cudaMemcpyToSymbol);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyToSymbol():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyToSymbol(
		symbol, 
		src, 
		count, 
		offset, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}