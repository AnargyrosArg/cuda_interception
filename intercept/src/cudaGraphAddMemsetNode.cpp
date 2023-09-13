#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddMemsetNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
const struct cudaMemsetParams *
);
extern "C"
{
	cudaError_t cudaGraphAddMemsetNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, const struct cudaMemsetParams *pMemsetParams) {
		fprintf(stderr, "===============\ncudaGraphAddMemsetNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddMemsetNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddMemsetNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			const struct cudaMemsetParams *)
			) dlsym(RTLD_NEXT, "cudaGraphAddMemsetNode");
			fprintf(stderr, "original_cudaGraphAddMemsetNode:%p\n", original_cudaGraphAddMemsetNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddMemsetNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddMemsetNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		pMemsetParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}