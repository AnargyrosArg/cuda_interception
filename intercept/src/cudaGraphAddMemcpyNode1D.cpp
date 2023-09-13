#include <cuda_runtime_api.h>
#include <dlfcn.h>
#include <iostream>
cudaError_t (*original_cudaGraphAddMemcpyNode1D)(
cudaGraphNode_t *, 
cudaGraph_t, 
const cudaGraphNode_t *, 
size_t, 
void *, 
const void *, 
size_t, 
enum cudaMemcpyKind
);
extern "C"
{
	cudaError_t cudaGraphAddMemcpyNode1D(cudaGraphNode_t *pGraphNode, cudaGraph_t graph, const cudaGraphNode_t *pDependencies, size_t numDependencies, void *dst, const void *src, size_t count, enum cudaMemcpyKind kind) {
		fprintf(stderr, "===============\ncudaGraphAddMemcpyNode1D()\n");
		char* __dlerror;
		if (!original_cudaGraphAddMemcpyNode1D)
		{
			//fetch the original function addr using dlsym
			original_cudaGraphAddMemcpyNode1D = (cudaError_t (*)(
			cudaGraphNode_t *, 
			cudaGraph_t, 
			const cudaGraphNode_t *, 
			size_t, 
			void *, 
			const void *, 
			size_t, 
			enum cudaMemcpyKind)
			) dlsym(RTLD_NEXT, "cudaGraphAddMemcpyNode1D");
			fprintf(stderr, "original_cudaGraphAddMemcpyNode1D:%p\n", original_cudaGraphAddMemcpyNode1D);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cudaGraphAddMemcpyNode1D():%s\n", __dlerror);
			fflush(stderr);
		}
		cudaError_t retval = original_cudaGraphAddMemcpyNode1D(
		pGraphNode, 
		graph, 
		pDependencies, 
		numDependencies, 
		dst, 
		src, 
		count, 
		kind
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}