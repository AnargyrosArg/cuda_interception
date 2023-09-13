#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphGetRootNodes)(
cudaGraph_t, 
cudaGraphNode_t *, 
size_t *
);
extern "C"
{
	cudaError_t cudaGraphGetRootNodes(cudaGraph_t graph, cudaGraphNode_t *pRootNodes, size_t *pNumRootNodes) {
		fprintf(stderr, "===============\ncudaGraphGetRootNodes()\n");
		char* __dlerror;
		if (!original_cudaGraphGetRootNodes)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphGetRootNodes = (cudaError_t (*)(
			cudaGraph_t, 
			cudaGraphNode_t *, 
			size_t *)
			) dlsym(RTLD_NEXT, "cudaGraphGetRootNodes");
			fprintf(stderr, "original_cudaGraphGetRootNodes:%p\n", original_cudaGraphGetRootNodes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphGetRootNodes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphGetRootNodes(
		graph, 
		pRootNodes, 
		pNumRootNodes
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}