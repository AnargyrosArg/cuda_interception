#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphNodeGetDependencies)(
CUgraphNode, 
CUgraphNode *, 
size_t *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphNodeGetDependencies(CUgraphNode hNode, CUgraphNode *dependencies, size_t *numDependencies) {
		fprintf(stderr, "cuGraphNodeGetDependencies()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphNodeGetDependencies)
		{
			//fetch the original function addr using dlsym
			original_cuGraphNodeGetDependencies = (CUresult (*)(
			CUgraphNode, 
			CUgraphNode *, 
			size_t *)
			) dlsym(original_libcuda_handle, "cuGraphNodeGetDependencies");
			fprintf(stderr, "original_cuGraphNodeGetDependencies:%p\n", original_cuGraphNodeGetDependencies);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphNodeGetDependencies():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphNodeGetDependencies(
		hNode, 
		dependencies, 
		numDependencies
		);
	}
}