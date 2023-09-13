#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphExecKernelNodeSetParams)(
cudaGraphExec_t, 
cudaGraphNode_t, 
const struct cudaKernelNodeParams *
);
extern "C"
{
	cudaError_t cudaGraphExecKernelNodeSetParams(cudaGraphExec_t hGraphExec, cudaGraphNode_t node, const struct cudaKernelNodeParams *pNodeParams) {
		fprintf(stderr, "===============\ncudaGraphExecKernelNodeSetParams()\n");
		char* __dlerror;
		if (!original_cudaGraphExecKernelNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphExecKernelNodeSetParams = (cudaError_t (*)(
			cudaGraphExec_t, 
			cudaGraphNode_t, 
			const struct cudaKernelNodeParams *)
			) dlsym(RTLD_NEXT, "cudaGraphExecKernelNodeSetParams");
			fprintf(stderr, "original_cudaGraphExecKernelNodeSetParams:%p\n", original_cudaGraphExecKernelNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphExecKernelNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphExecKernelNodeSetParams(
		hGraphExec, 
		node, 
		pNodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}