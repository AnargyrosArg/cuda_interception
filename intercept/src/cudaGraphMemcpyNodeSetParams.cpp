#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemcpyNodeSetParams)(
cudaGraphNode_t, 
const struct cudaMemcpy3DParms *
);
extern "C"
{
	cudaError_t cudaGraphMemcpyNodeSetParams(cudaGraphNode_t node, const struct cudaMemcpy3DParms *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphMemcpyNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphMemcpyNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemcpyNodeSetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			const struct cudaMemcpy3DParms *)
			) dlsym(RTLD_NEXT, "cudaGraphMemcpyNodeSetParams");
			fprintf(stderr, "original_cudaGraphMemcpyNodeSetParams:%p\n", original_cudaGraphMemcpyNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemcpyNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemcpyNodeSetParams(
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}