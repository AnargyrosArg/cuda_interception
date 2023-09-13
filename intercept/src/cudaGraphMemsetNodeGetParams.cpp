#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphMemsetNodeGetParams)(
cudaGraphNode_t, 
struct cudaMemsetParams *
);
extern "C"
{
	cudaError_t cudaGraphMemsetNodeGetParams(cudaGraphNode_t node, struct cudaMemsetParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphMemsetNodeGetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphMemsetNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphMemsetNodeGetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			struct cudaMemsetParams *)
			) dlsym(RTLD_NEXT, "cudaGraphMemsetNodeGetParams");
			fprintf(stderr, "original_cudaGraphMemsetNodeGetParams:%p\n", original_cudaGraphMemsetNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphMemsetNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphMemsetNodeGetParams(
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}