#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphGetEdges)(
CUgraph, 
CUgraphNode *, 
CUgraphNode *, 
size_t *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphGetEdges(CUgraph hGraph, CUgraphNode *from, CUgraphNode *to, size_t *numEdges) {
		fprintf(stderr, "cuGraphGetEdges()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphGetEdges)
		{
			//fetch the original function addr using dlsym
			original_cuGraphGetEdges = (CUresult (*)(
			CUgraph, 
			CUgraphNode *, 
			CUgraphNode *, 
			size_t *)
			) dlsym(original_libcuda_handle, "cuGraphGetEdges");
			fprintf(stderr, "original_cuGraphGetEdges:%p\n", original_cuGraphGetEdges);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphGetEdges():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphGetEdges(
		hGraph, 
		from, 
		to, 
		numEdges
		);
	}
}