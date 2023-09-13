#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddMemFreeNode)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
void *
);
extern "C"
{
	cudaError_t cudaGraphAddMemFreeNode(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, void *dptr) {
		fprintf(stderr, "===============\ncudaGraphAddMemFreeNode()\n");
		char* __dlerror;
		if (!original_cudaGraphAddMemFreeNode)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddMemFreeNode = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			void *)
			) dlsym(RTLD_NEXT, "cudaGraphAddMemFreeNode");
			fprintf(stderr, "original_cudaGraphAddMemFreeNode:%p\n", original_cudaGraphAddMemFreeNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddMemFreeNode():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddMemFreeNode(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		dptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}