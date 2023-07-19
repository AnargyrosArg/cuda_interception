#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphGetNodes)(
CUgraph, 
CUgraphNode *, 
size_t *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphGetNodes(CUgraph hGraph, CUgraphNode *nodes, size_t *numNodes) {
		fprintf(stderr, "cuGraphGetNodes()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphGetNodes)
		{
			//fetch the original function addr using dlsym
			original_cuGraphGetNodes = (CUresult (*)(
			CUgraph, 
			CUgraphNode *, 
			size_t *)
			) dlsym(original_libcuda_handle, "cuGraphGetNodes");
			fprintf(stderr, "original_cuGraphGetNodes:%p\n", original_cuGraphGetNodes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphGetNodes():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphGetNodes(
		hGraph, 
		nodes, 
		numNodes
		);
	}
}