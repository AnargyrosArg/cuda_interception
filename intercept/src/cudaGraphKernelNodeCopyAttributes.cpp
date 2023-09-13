#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphKernelNodeCopyAttributes)(
cudaGraphNode_t, 
cudaGraphNode_t
);
extern "C"
{
	cudaError_t cudaGraphKernelNodeCopyAttributes(cudaGraphNode_t hSrc, cudaGraphNode_t hDst) {
		fprintf(stderr, "===============\ncudaGraphKernelNodeCopyAttributes()\n");
		char* __dlerror;
		if (!original_cudaGraphKernelNodeCopyAttributes)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphKernelNodeCopyAttributes = (cudaError_t (*)(
			cudaGraphNode_t, 
			cudaGraphNode_t)
			) dlsym(RTLD_NEXT, "cudaGraphKernelNodeCopyAttributes");
			fprintf(stderr, "original_cudaGraphKernelNodeCopyAttributes:%p\n", original_cudaGraphKernelNodeCopyAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphKernelNodeCopyAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphKernelNodeCopyAttributes(
		hSrc, 
		hDst
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}