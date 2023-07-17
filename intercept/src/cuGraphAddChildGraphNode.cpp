#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddChildGraphNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
CUgraph
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddChildGraphNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, CUgraph childGraph) {
		fprintf(stderr, "cuGraphAddChildGraphNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddChildGraphNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddChildGraphNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			CUgraph)
			) dlsym(original_libcuda_handle, "cuGraphAddChildGraphNode");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddChildGraphNode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphAddChildGraphNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		childGraph
		);
	}
}