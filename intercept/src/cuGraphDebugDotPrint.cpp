#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphDebugDotPrint)(
CUgraph, 
const char *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphDebugDotPrint(CUgraph hGraph, const char *path, unsigned int flags) {
		fprintf(stderr, "===============\ncuGraphDebugDotPrint()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphDebugDotPrint)
		{
			//fetch the original function addr using dlsym
			original_cuGraphDebugDotPrint = (CUresult (*)(
			CUgraph, 
			const char *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphDebugDotPrint");
			fprintf(stderr, "original_cuGraphDebugDotPrint:%p\n", original_cuGraphDebugDotPrint);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphDebugDotPrint():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphDebugDotPrint(
		hGraph, 
		path, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}