#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphNodeGetDependentNodes)(
CUgraphNode, 
CUgraphNode *, 
size_t *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphNodeGetDependentNodes(CUgraphNode hNode, CUgraphNode *dependentNodes, size_t *numDependentNodes) {
		fprintf(stderr, "===============\ncuGraphNodeGetDependentNodes()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphNodeGetDependentNodes)
		{
			//fetch the original function addr using dlsym
			original_cuGraphNodeGetDependentNodes = (CUresult (*)(
			CUgraphNode, 
			CUgraphNode *, 
			size_t *)
			) dlsym(original_libcuda_handle, "cuGraphNodeGetDependentNodes");
			fprintf(stderr, "original_cuGraphNodeGetDependentNodes:%p\n", original_cuGraphNodeGetDependentNodes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphNodeGetDependentNodes():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphNodeGetDependentNodes(
		hNode, 
		dependentNodes, 
		numDependentNodes
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}