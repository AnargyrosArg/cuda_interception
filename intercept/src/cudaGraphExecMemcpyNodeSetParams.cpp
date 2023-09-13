#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecMemcpyNodeSetParams)(
cudaGraphExec_t, 
cudaGraphNode_t, 
const struct cudaMemcpy3DParms *
);
extern "C"
{
	cudaError_t cudaGraphExecMemcpyNodeSetParams(cudaGraphExec_t hGraphExec, cudaGraphNode_t node, const struct cudaMemcpy3DParms *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphExecMemcpyNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExecMemcpyNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecMemcpyNodeSetParams = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			const struct cudaMemcpy3DParms *)
			) dlsym(RTLD_NEXT, "cudaGraphExecMemcpyNodeSetParams");
			fprintf(stderr, "original_cudaGraphExecMemcpyNodeSetParams:%p\n", original_cudaGraphExecMemcpyNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecMemcpyNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecMemcpyNodeSetParams(
		hGraphExec, 
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}