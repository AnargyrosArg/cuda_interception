#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecMemsetNodeSetParams)(
cudaGraphExec_t, 
cudaGraphNode_t, 
const struct cudaMemsetParams *
);
extern "C"
{
	cudaError_t cudaGraphExecMemsetNodeSetParams(cudaGraphExec_t hGraphExec, cudaGraphNode_t node, const struct cudaMemsetParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphExecMemsetNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExecMemsetNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecMemsetNodeSetParams = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			const struct cudaMemsetParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExecMemsetNodeSetParams");
			fprintf(stderr, "original_cudaGraphExecMemsetNodeSetParams:%p\n", original_cudaGraphExecMemsetNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecMemsetNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecMemsetNodeSetParams(
		hGraphExec, 
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}