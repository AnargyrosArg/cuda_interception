#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecMemcpyNodeSetParamsFromSymbol)(
cudaGraphExec_t, 
cudaGraphNode_t, 
void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaGraphExecMemcpyNodeSetParamsFromSymbol(cudaGraphExec_t hGraphExec, cudaGraphNode_t node, void *dst, const void *symbol, size_t count, size_t offset, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaGraphExecMemcpyNodeSetParamsFromSymbol()\n");
		char* __dlerror;
		if (!original_cudaGraphExecMemcpyNodeSetParamsFromSymbol)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecMemcpyNodeSetParamsFromSymbol = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaGraphExecMemcpyNodeSetParamsFromSymbol");
			fprintf(stderr, "original_cudaGraphExecMemcpyNodeSetParamsFromSymbol:%p\n", original_cudaGraphExecMemcpyNodeSetParamsFromSymbol);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecMemcpyNodeSetParamsFromSymbol():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecMemcpyNodeSetParamsFromSymbol(
		hGraphExec, 
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