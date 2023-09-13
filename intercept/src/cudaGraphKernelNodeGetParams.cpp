#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphKernelNodeGetParams)(
cudaGraphNode_t, 
struct cudaKernelNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphKernelNodeGetParams(cudaGraphNode_t node, struct cudaKernelNodeParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphKernelNodeGetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphKernelNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphKernelNodeGetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			struct cudaKernelNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphKernelNodeGetParams");
			fprintf(stderr, "original_cudaGraphKernelNodeGetParams:%p\n", original_cudaGraphKernelNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphKernelNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphKernelNodeGetParams(
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}