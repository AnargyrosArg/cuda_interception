#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddMemcpyNodeFromSymbol)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
void *, 
const void *, 
size_t, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaGraphAddMemcpyNodeFromSymbol(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, void *dst, const void *symbol, size_t count, size_t offset, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaGraphAddMemcpyNodeFromSymbol()\n");
		char* __dlerror;
		if (!original_cudaGraphAddMemcpyNodeFromSymbol)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddMemcpyNodeFromSymbol = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			void *, 
			const void *, 
			size_t, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaGraphAddMemcpyNodeFromSymbol");
			fprintf(stderr, "original_cudaGraphAddMemcpyNodeFromSymbol:%p\n", original_cudaGraphAddMemcpyNodeFromSymbol);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddMemcpyNodeFromSymbol():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddMemcpyNodeFromSymbol(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		dst, 
		symbol, 
		count, 
		offset, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}