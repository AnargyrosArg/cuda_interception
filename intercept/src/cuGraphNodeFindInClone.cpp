#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphNodeFindInClone)(
CUgraphNode *, 
CUgraphNode, 
CUgraph
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphNodeFindInClone(CUgraphNode *phNode, CUgraphNode hOriginalNode, CUgraph hClonedGraph) {
		fprintf(stderr, "cuGraphNodeFindInClone()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphNodeFindInClone)
		{
			//fetch the original function addr using dlsym
			original_cuGraphNodeFindInClone = (CUresult (*)(
			CUgraphNode *, 
			CUgraphNode, 
			CUgraph)
			) dlsym(original_libcuda_handle, "cuGraphNodeFindInClone");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphNodeFindInClone():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphNodeFindInClone(
		phNode, 
		hOriginalNode, 
		hClonedGraph
		);
	}
}