#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecChildGraphNodeSetParams)(
cudaGraphExec_t, 
cudaGraphNode_t, 
cudaGraph_t
);
extern "C"
{
	cudaError_t cudaGraphExecChildGraphNodeSetParams(cudaGraphExec_t hGraphExec, cudaGraphNode_t node, cudaGraph_t childGraph) {
		fprintf(stderr, "===============\ncudaGraphExecChildGraphNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExecChildGraphNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecChildGraphNodeSetParams = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			cudaGraph_t)
			) dlsym(RTLD_NEXT, "cudaGraphExecChildGraphNodeSetParams");
			fprintf(stderr, "original_cudaGraphExecChildGraphNodeSetParams:%p\n", original_cudaGraphExecChildGraphNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecChildGraphNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecChildGraphNodeSetParams(
		hGraphExec, 
		node, 
		childGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}