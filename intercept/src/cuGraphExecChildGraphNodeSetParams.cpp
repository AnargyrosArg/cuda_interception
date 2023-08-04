#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecChildGraphNodeSetParams)(
CUgraphExec, 
CUgraphNode, 
CUgraph
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecChildGraphNodeSetParams(CUgraphExec hGraphExec, CUgraphNode hNode, CUgraph childGraph) {
		fprintf(stderr, "===============\ncuGraphExecChildGraphNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecChildGraphNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecChildGraphNodeSetParams = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			CUgraph)
			) dlsym(original_libcuda_handle, "cuGraphExecChildGraphNodeSetParams");
			fprintf(stderr, "original_cuGraphExecChildGraphNodeSetParams:%p\n", original_cuGraphExecChildGraphNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecChildGraphNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphExecChildGraphNodeSetParams(
		hGraphExec, 
		hNode, 
		childGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}