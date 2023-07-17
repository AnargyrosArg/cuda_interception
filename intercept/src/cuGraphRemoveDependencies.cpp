#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphRemoveDependencies)(
CUgraph, 
const CUgraphNode *, 
const CUgraphNode *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphRemoveDependencies(CUgraph hGraph, const CUgraphNode *from, const CUgraphNode *to, size_t numDependencies) {
		fprintf(stderr, "cuGraphRemoveDependencies()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphRemoveDependencies)
		{
			//fetch the original function addr using dlsym
			original_cuGraphRemoveDependencies = (CUresult (*)(
			CUgraph, 
			const CUgraphNode *, 
			const CUgraphNode *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuGraphRemoveDependencies");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphRemoveDependencies():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphRemoveDependencies(
		hGraph, 
		from, 
		to, 
		numDependencies
		);
	}
}