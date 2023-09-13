#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphGetNodes)(
cudaGraph_t, 
cudaGraphNode_t *, 
size_t *
);
extern "C"
{
	cudaError_t cudaGraphGetNodes(cudaGraph_t graph, cudaGraphNode_t *nodes, size_t *numNodes) {
		fprintf(stderr, "===============\ncudaGraphGetNodes()\n");
		char* __dlerror;
		if (!original_cudaGraphGetNodes)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphGetNodes = (cudaError_t (*)(
			cudaGraph_t, 
			cudaGraphNode_t *, 
			size_t *)
			) dlsym(RTLD_NEXT, "cudaGraphGetNodes");
			fprintf(stderr, "original_cudaGraphGetNodes:%p\n", original_cudaGraphGetNodes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphGetNodes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphGetNodes(
		graph, 
		nodes, 
		numNodes
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}