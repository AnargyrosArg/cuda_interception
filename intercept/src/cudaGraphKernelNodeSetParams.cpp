#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphKernelNodeSetParams)(
cudaGraphNode_t, 
const struct cudaKernelNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphKernelNodeSetParams(cudaGraphNode_t node, const struct cudaKernelNodeParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphKernelNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphKernelNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphKernelNodeSetParams = (cudaError_t (*)(
			cudaGraphNode_t, 
			const struct cudaKernelNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphKernelNodeSetParams");
			fprintf(stderr, "original_cudaGraphKernelNodeSetParams:%p\n", original_cudaGraphKernelNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphKernelNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphKernelNodeSetParams(
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}