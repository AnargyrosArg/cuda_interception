#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddMemAllocNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
struct cudaMemAllocNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphAddMemAllocNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, struct cudaMemAllocNodeParams *nodeParams) {
		fprintf(stderr, "===============\ncudaGraphAddMemAllocNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddMemAllocNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddMemAllocNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			struct cudaMemAllocNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphAddMemAllocNode");
			fprintf(stderr, "original_cudaGraphAddMemAllocNode:%p\n", original_cudaGraphAddMemAllocNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddMemAllocNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddMemAllocNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}