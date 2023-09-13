#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemcpyNodeSetParamsFromSymbol)(
cudaGraphNode_t, 
void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaGraphMemcpyNodeSetParamsFromSymbol(cudaGraphNode_t node, void *dst, const void *symbol, size_t count, size_t offset, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaGraphMemcpyNodeSetParamsFromSymbol()\n");
		char* __dlerror;
		if (!original_cudaGraphMemcpyNodeSetParamsFromSymbol)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemcpyNodeSetParamsFromSymbol = (cudaError_t (*)(
			cudaGraphNode_t, 
			void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaGraphMemcpyNodeSetParamsFromSymbol");
			fprintf(stderr, "original_cudaGraphMemcpyNodeSetParamsFromSymbol:%p\n", original_cudaGraphMemcpyNodeSetParamsFromSymbol);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemcpyNodeSetParamsFromSymbol():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemcpyNodeSetParamsFromSymbol(
		node, 
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