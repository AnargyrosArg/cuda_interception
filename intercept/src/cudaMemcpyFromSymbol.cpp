#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaMemcpyFromSymbol)(
void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaMemcpyFromSymbol(void *dst, const void *symbol, size_t count, size_t offset, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaMemcpyFromSymbol()\n");
		char* __dlerror;
		if (!original_cudaMemcpyFromSymbol)
		{
			//fetch the original function addr using dlsym
			original_cudaMemcpyFromSymbol = (cudaError_t (*)(
			void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaMemcpyFromSymbol");
			fprintf(stderr, "original_cudaMemcpyFromSymbol:%p\n", original_cudaMemcpyFromSymbol);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaMemcpyFromSymbol():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaMemcpyFromSymbol(
		dst, 
		symbol, 
		count, 
		offset, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}