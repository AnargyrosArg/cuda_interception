#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphCreate)(
CUgraph *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphCreate(CUgraph *phGraph, unsigned int flags) {
		fprintf(stderr, "===============\ncuGraphCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphCreate)
		{
			//fetch the original function addr using dlsym
			original_cuGraphCreate = (CUresult (*)(
			CUgraph *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuGraphCreate");
			fprintf(stderr, "original_cuGraphCreate:%p\n", original_cuGraphCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphCreate(
		phGraph, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}