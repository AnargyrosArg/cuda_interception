#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecMemcpyNodeSetParamsToSymbol)(
cudaGraphExec_t, 
cudaGraphNode_t, 
const void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaGraphExecMemcpyNodeSetParamsToSymbol(cudaGraphExec_t hGraphExec, cudaGraphNode_t node, const void *symbol, const void *src, size_t count, size_t offset, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaGraphExecMemcpyNodeSetParamsToSymbol()\n");
		char* __dlerror;
		if (!original_cudaGraphExecMemcpyNodeSetParamsToSymbol)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecMemcpyNodeSetParamsToSymbol = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			const void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaGraphExecMemcpyNodeSetParamsToSymbol");
			fprintf(stderr, "original_cudaGraphExecMemcpyNodeSetParamsToSymbol:%p\n", original_cudaGraphExecMemcpyNodeSetParamsToSymbol);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecMemcpyNodeSetParamsToSymbol():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecMemcpyNodeSetParamsToSymbol(
		hGraphExec, 
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