#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphClone)(
CUgraph *, 
CUgraph
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphClone(CUgraph *phGraphClone, CUgraph originalGraph) {
		fprintf(stderr, "===============\ncuGraphClone()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphClone)
		{
			//fetch the original function addr using dlsym
			original_cuGraphClone = (CUresult (*)(
			CUgraph *, 
			CUgraph)
			) dlsym(original_libcuda_handle, "cuGraphClone");
			fprintf(stderr, "original_cuGraphClone:%p\n", original_cuGraphClone);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphClone():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphClone(
		phGraphClone, 
		originalGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}