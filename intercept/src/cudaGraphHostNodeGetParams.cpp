#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphHostNodeGetParams)(
cudaGraphNode_t, 
struct cudaHostNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphHostNodeGetParams(cudaGraphNode_t node, struct cudaHostNodeParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphHostNodeGetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphHostNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphHostNodeGetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			struct cudaHostNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphHostNodeGetParams");
			fprintf(stderr, "original_cudaGraphHostNodeGetParams:%p\n", original_cudaGraphHostNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphHostNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphHostNodeGetParams(
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}