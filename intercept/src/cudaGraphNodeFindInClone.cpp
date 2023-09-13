#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphNodeFindInClone)(
cudaGraphNode_t *, 
cudaGraphNode_t, 
cudaGraph_t
);
extern "C"
{
	cudaError_t cudaGraphNodeFindInClone(cudaGraphNode_t *pNode, cudaGraphNode_t originalNode, cudaGraph_t clonedGraph) {
		fprintf(stderr, "===============\ncudaGraphNodeFindInClone()\n");
		char* __dlerror;
		if (!original_cudaGraphNodeFindInClone)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphNodeFindInClone = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraphNode_t, 
			cudaGraph_t)
			) dlsym(RTLD_NEXT, "cudaGraphNodeFindInClone");
			fprintf(stderr, "original_cudaGraphNodeFindInClone:%p\n", original_cudaGraphNodeFindInClone);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphNodeFindInClone():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphNodeFindInClone(
		pNode, 
		originalNode, 
		clonedGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}