#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemcpyNodeSetParamsToSymbol)(
cudaGraphNode_t, 
const void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaGraphMemcpyNodeSetParamsToSymbol(cudaGraphNode_t node, const void *symbol, const void *src, size_t count, size_t offset, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaGraphMemcpyNodeSetParamsToSymbol()\n");
		char* __dlerror;
		if (!original_cudaGraphMemcpyNodeSetParamsToSymbol)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemcpyNodeSetParamsToSymbol = (cudaError_t (*)(
			cudaGraphNode_t, 
			const void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaGraphMemcpyNodeSetParamsToSymbol");
			fprintf(stderr, "original_cudaGraphMemcpyNodeSetParamsToSymbol:%p\n", original_cudaGraphMemcpyNodeSetParamsToSymbol);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemcpyNodeSetParamsToSymbol():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemcpyNodeSetParamsToSymbol(
		node, 
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