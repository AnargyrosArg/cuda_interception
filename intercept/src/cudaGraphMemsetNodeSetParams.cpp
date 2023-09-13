#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemsetNodeSetParams)(
cudaGraphNode_t, 
const struct cudaMemsetParams *
);
extern "C"
{
	cudaError_t cudaGraphMemsetNodeSetParams(cudaGraphNode_t node, const struct cudaMemsetParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphMemsetNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphMemsetNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemsetNodeSetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			const struct cudaMemsetParams *)
			) dlsym(RTLD_NEXT, "cudaGraphMemsetNodeSetParams");
			fprintf(stderr, "original_cudaGraphMemsetNodeSetParams:%p\n", original_cudaGraphMemsetNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemsetNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemsetNodeSetParams(
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}