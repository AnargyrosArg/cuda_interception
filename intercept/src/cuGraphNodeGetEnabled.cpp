#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphNodeGetEnabled)(
CUgraphExec, 
CUgraphNode, 
unsigned int *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphNodeGetEnabled(CUgraphExec hGraphExec, CUgraphNode hNode, unsigned int *isEnabled) {
		fprintf(stderr, "===============\ncuGraphNodeGetEnabled()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphNodeGetEnabled)
		{
			//fetch the original function addr using dlsym
			original_cuGraphNodeGetEnabled = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			unsigned int *)
			) dlsym(original_libcuda_handle, "cuGraphNodeGetEnabled");
			fprintf(stderr, "original_cuGraphNodeGetEnabled:%p\n", original_cuGraphNodeGetEnabled);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphNodeGetEnabled():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphNodeGetEnabled(
		hGraphExec, 
		hNode, 
		isEnabled
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}