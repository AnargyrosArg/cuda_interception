#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphNodeSetEnabled)(
CUgraphExec, 
CUgraphNode, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphNodeSetEnabled(CUgraphExec hGraphExec, CUgraphNode hNode, unsigned int isEnabled) {
		fprintf(stderr, "cuGraphNodeSetEnabled()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphNodeSetEnabled)
		{
			//fetch the original function addr using dlsym
			original_cuGraphNodeSetEnabled = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphNodeSetEnabled");
			fprintf(stderr, "original_cuGraphNodeSetEnabled:%p\n", original_cuGraphNodeSetEnabled);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphNodeSetEnabled():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphNodeSetEnabled(
		hGraphExec, 
		hNode, 
		isEnabled
		);
	}
}