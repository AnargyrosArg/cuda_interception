#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphNodeGetType)(
CUgraphNode, 
CUgraphNodeType *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphNodeGetType(CUgraphNode hNode, CUgraphNodeType *type) {
		fprintf(stderr, "===============\ncuGraphNodeGetType()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphNodeGetType)
		{
			//fetch the original function addr using dlsym
			original_cuGraphNodeGetType = (CUresult (*)(
			CUgraphNode, 
			CUgraphNodeType *)
			) dlsym(original_libcuda_handle, "cuGraphNodeGetType");
			fprintf(stderr, "original_cuGraphNodeGetType:%p\n", original_cuGraphNodeGetType);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphNodeGetType():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphNodeGetType(
		hNode, 
		type
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}