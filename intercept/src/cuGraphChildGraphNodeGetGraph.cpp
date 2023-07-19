#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphChildGraphNodeGetGraph)(
CUgraphNode, 
CUgraph *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphChildGraphNodeGetGraph(CUgraphNode hNode, CUgraph *phGraph) {
		fprintf(stderr, "cuGraphChildGraphNodeGetGraph()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphChildGraphNodeGetGraph)
		{
			//fetch the original function addr using dlsym
			original_cuGraphChildGraphNodeGetGraph = (CUresult (*)(
			CUgraphNode, 
			CUgraph *)
			) dlsym(original_libcuda_handle, "cuGraphChildGraphNodeGetGraph");
			fprintf(stderr, "original_cuGraphChildGraphNodeGetGraph:%p\n", original_cuGraphChildGraphNodeGetGraph);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphChildGraphNodeGetGraph():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphChildGraphNodeGetGraph(
		hNode, 
		phGraph
		);
	}
}