#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemcpyNodeGetParams)(
cudaGraphNode_t, 
struct cudaMemcpy3DParms *
);
extern "C"
{
	cudaError_t cudaGraphMemcpyNodeGetParams(cudaGraphNode_t node, struct cudaMemcpy3DParms *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphMemcpyNodeGetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphMemcpyNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemcpyNodeGetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			struct cudaMemcpy3DParms *)
			) dlsym(RTLD_NEXT, "cudaGraphMemcpyNodeGetParams");
			fprintf(stderr, "original_cudaGraphMemcpyNodeGetParams:%p\n", original_cudaGraphMemcpyNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemcpyNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemcpyNodeGetParams(
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}