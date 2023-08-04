#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphMemFreeNodeGetParams)(
CUgraphNode, 
CUdeviceptr *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphMemFreeNodeGetParams(CUgraphNode hNode, CUdeviceptr *dptr_out) {
		fprintf(stderr, "===============\ncuGraphMemFreeNodeGetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphMemFreeNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphMemFreeNodeGetParams = (CUresult (*)(
			CUgraphNode, 
			CUdeviceptr *)
			) dlsym(original_libcuda_handle, "cuGraphMemFreeNodeGetParams");
			fprintf(stderr, "original_cuGraphMemFreeNodeGetParams:%p\n", original_cuGraphMemFreeNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphMemFreeNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphMemFreeNodeGetParams(
		hNode, 
		dptr_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}