#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecDestroy)(
CUgraphExec
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecDestroy(CUgraphExec hGraphExec) {
		fprintf(stderr, "===============\ncuGraphExecDestroy()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecDestroy)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecDestroy = (CUresult (*)(
			CUgraphExec)
			) dlsym(original_libcuda_handle, "cuGraphExecDestroy");
			fprintf(stderr, "original_cuGraphExecDestroy:%p\n", original_cuGraphExecDestroy);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecDestroy():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphExecDestroy(
		hGraphExec
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}