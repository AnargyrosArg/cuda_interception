#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddEmptyNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddEmptyNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies) {
		fprintf(stderr, "===============\ncuGraphAddEmptyNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddEmptyNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddEmptyNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuGraphAddEmptyNode");
			fprintf(stderr, "original_cuGraphAddEmptyNode:%p\n", original_cuGraphAddEmptyNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddEmptyNode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphAddEmptyNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}