#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddHostNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
const CUDA_HOST_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddHostNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, const CUDA_HOST_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphAddHostNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddHostNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddHostNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			const CUDA_HOST_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphAddHostNode");
			fprintf(stderr, "original_cuGraphAddHostNode:%p\n", original_cuGraphAddHostNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddHostNode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphAddHostNode(
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