#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecHostNodeSetParams)(
cudaGraphExec_t, 
cudaGraphNode_t, 
const struct cudaHostNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphExecHostNodeSetParams(cudaGraphExec_t hGraphExec, cudaGraphNode_t node, const struct cudaHostNodeParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphExecHostNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExecHostNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecHostNodeSetParams = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			const struct cudaHostNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExecHostNodeSetParams");
			fprintf(stderr, "original_cudaGraphExecHostNodeSetParams:%p\n", original_cudaGraphExecHostNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecHostNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecHostNodeSetParams(
		hGraphExec, 
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}