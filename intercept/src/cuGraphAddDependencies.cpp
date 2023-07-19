#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddDependencies)(
CUgraph, 
const CUgraphNode *, 
const CUgraphNode *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddDependencies(CUgraph hGraph, const CUgraphNode *from, const CUgraphNode *to, size_t numDependencies) {
		fprintf(stderr, "cuGraphAddDependencies()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddDependencies)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddDependencies = (CUresult (*)(
			CUgraph, 
			const CUgraphNode *, 
			const CUgraphNode *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuGraphAddDependencies");
			fprintf(stderr, "original_cuGraphAddDependencies:%p\n", original_cuGraphAddDependencies);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddDependencies():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphAddDependencies(
		hGraph, 
		from, 
		to, 
		numDependencies
		);
	}
}