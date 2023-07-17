#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphGetRootNodes)(
CUgraph, 
CUgraphNode *, 
size_t *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphGetRootNodes(CUgraph hGraph, CUgraphNode *rootNodes, size_t *numRootNodes) {
		fprintf(stderr, "cuGraphGetRootNodes()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphGetRootNodes)
		{
			//fetch the original function addr using dlsym
			original_cuGraphGetRootNodes = (CUresult (*)(
			CUgraph, 
			CUgraphNode *, 
			size_t *)
			) dlsym(original_libcuda_handle, "cuGraphGetRootNodes");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphGetRootNodes():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphGetRootNodes(
		hGraph, 
		rootNodes, 
		numRootNodes
		);
	}
}