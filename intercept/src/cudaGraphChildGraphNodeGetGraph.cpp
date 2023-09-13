#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphChildGraphNodeGetGraph)(
cudaGraphNode_t, 
cudaGraph_t *
);
extern "C"
{
	cudaError_t cudaGraphChildGraphNodeGetGraph(cudaGraphNode_t node, cudaGraph_t *pGraph) {
		fprintf(stderr, "===============\ncudaGraphChildGraphNodeGetGraph()\n");
		char* __dlerror;
		if (!original_cudaGraphChildGraphNodeGetGraph)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphChildGraphNodeGetGraph = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaGraph_t *)
			) dlsym(RTLD_NEXT, "cudaGraphChildGraphNodeGetGraph");
			fprintf(stderr, "original_cudaGraphChildGraphNodeGetGraph:%p\n", original_cudaGraphChildGraphNodeGetGraph);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphChildGraphNodeGetGraph():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphChildGraphNodeGetGraph(
		node, 
		pGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}