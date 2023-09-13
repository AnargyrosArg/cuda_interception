#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphHostNodeSetParams)(
cudaGraphNode_t, 
const struct cudaHostNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphHostNodeSetParams(cudaGraphNode_t node, const struct cudaHostNodeParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphHostNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphHostNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphHostNodeSetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			const struct cudaHostNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphHostNodeSetParams");
			fprintf(stderr, "original_cudaGraphHostNodeSetParams:%p\n", original_cudaGraphHostNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphHostNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphHostNodeSetParams(
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}