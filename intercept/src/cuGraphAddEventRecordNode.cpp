#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddEventRecordNode)(
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
	CUresult cuGraphAddEventRecordNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, CUevent event) {
		fprintf(stderr, "===============\ncuGraphAddEventRecordNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddEventRecordNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddEventRecordNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			CUevent)
			) dlsym(original_libcuda_handle, "cuGraphAddEventRecordNode");
			fprintf(stderr, "original_cuGraphAddEventRecordNode:%p\n", original_cuGraphAddEventRecordNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddEventRecordNode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphAddEventRecordNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		event
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}