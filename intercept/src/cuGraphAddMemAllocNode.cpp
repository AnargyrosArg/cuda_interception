#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddMemAllocNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
CUDA_MEM_ALLOC_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddMemAllocNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, CUDA_MEM_ALLOC_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphAddMemAllocNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddMemAllocNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddMemAllocNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			CUDA_MEM_ALLOC_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphAddMemAllocNode");
			fprintf(stderr, "original_cuGraphAddMemAllocNode:%p\n", original_cuGraphAddMemAllocNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddMemAllocNode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphAddMemAllocNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}