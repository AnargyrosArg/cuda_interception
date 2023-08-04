#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddMemsetNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
const CUDA_MEMSET_NODE_PARAMS *, 
CUcontext
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddMemsetNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, const CUDA_MEMSET_NODE_PARAMS *memsetParams, CUcontext ctx) {
		fprintf(stderr, "===============\ncuGraphAddMemsetNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddMemsetNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddMemsetNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			const CUDA_MEMSET_NODE_PARAMS *, 
			CUcontext)
			) dlsym(original_libcuda_handle, "cuGraphAddMemsetNode");
			fprintf(stderr, "original_cuGraphAddMemsetNode:%p\n", original_cuGraphAddMemsetNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddMemsetNode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphAddMemsetNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		memsetParams, 
		ctx
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}