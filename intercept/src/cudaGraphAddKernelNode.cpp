#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddKernelNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
const struct cudaKernelNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphAddKernelNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, const struct cudaKernelNodeParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphAddKernelNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddKernelNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddKernelNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			const struct cudaKernelNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphAddKernelNode");
			fprintf(stderr, "original_cudaGraphAddKernelNode:%p\n", original_cudaGraphAddKernelNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddKernelNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddKernelNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}