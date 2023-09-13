#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphGetEdges)(
cudaGraph_t, 
cudaGraphNode_t *, 
cudaGraphNode_t *, 
size_t *
);
extern "C"
{
	cudaError_t cudaGraphGetEdges(cudaGraph_t graph, cudaGraphNode_t *from, cudaGraphNode_t *to, size_t *numEdges) {
		fprintf(stderr, "===============\ncudaGraphGetEdges()\n");
		char* __dlerror;
		if (!original_cudaGraphGetEdges)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphGetEdges = (cudaError_t (*)(
			cudaGraph_t, 
			cudaGraphNode_t *, 
			cudaGraphNode_t *, 
			size_t *)
			) dlsym(RTLD_NEXT, "cudaGraphGetEdges");
			fprintf(stderr, "original_cudaGraphGetEdges:%p\n", original_cudaGraphGetEdges);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphGetEdges():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphGetEdges(
		graph, 
		from, 
		to, 
		numEdges
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}