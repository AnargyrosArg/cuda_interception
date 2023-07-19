#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphInstantiateWithFlags)(
CUgraphExec *, 
CUgraph, 
unsigned long long
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphInstantiateWithFlags(CUgraphExec *phGraphExec, CUgraph hGraph, unsigned long long flags) {
		fprintf(stderr, "cuGraphInstantiateWithFlags()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphInstantiateWithFlags)
		{
			//fetch the original function addr using dlsym
			original_cuGraphInstantiateWithFlags = (CUresult (*)(
			CUgraphExec *, 
			CUgraph, 
			unsigned long long)
			) dlsym(original_libcuda_handle, "cuGraphInstantiateWithFlags");
			fprintf(stderr, "original_cuGraphInstantiateWithFlags:%p\n", original_cuGraphInstantiateWithFlags);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphInstantiateWithFlags():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphInstantiateWithFlags(
		phGraphExec, 
		hGraph, 
		flags
		);
	}
}