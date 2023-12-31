#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphDestroy)(
CUgraph
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphDestroy(CUgraph hGraph) {
		fprintf(stderr, "===============\ncuGraphDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuGraphDestroy = (CUresult (*)(
			CUgraph)
			) dlsym(original_libcuda_handle, "cuGraphDestroy");
			fprintf(stderr, "original_cuGraphDestroy:%p\n", original_cuGraphDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphDestroy(
		hGraph
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}