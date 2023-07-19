#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddEventWaitNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
CUevent
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddEventWaitNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, CUevent event) {
		fprintf(stderr, "cuGraphAddEventWaitNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddEventWaitNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddEventWaitNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			CUevent)
			) dlsym(original_libcuda_handle, "cuGraphAddEventWaitNode");
			fprintf(stderr, "original_cuGraphAddEventWaitNode:%p\n", original_cuGraphAddEventWaitNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddEventWaitNode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphAddEventWaitNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		event
		);
	}
}