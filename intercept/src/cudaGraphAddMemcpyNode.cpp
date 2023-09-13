#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddMemcpyNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
const struct cudaMemcpy3DParms *
);
extern "C"
{
	cudaError_t cudaGraphAddMemcpyNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, const struct cudaMemcpy3DParms *pCopyParams) {
		fprintf(stderr, "===============\ncudaGraphAddMemcpyNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddMemcpyNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddMemcpyNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			const struct cudaMemcpy3DParms *)
			) dlsym(RTLD_NEXT, "cudaGraphAddMemcpyNode");
			fprintf(stderr, "original_cudaGraphAddMemcpyNode:%p\n", original_cudaGraphAddMemcpyNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddMemcpyNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddMemcpyNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		pCopyParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}