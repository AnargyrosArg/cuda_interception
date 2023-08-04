#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphMemAllocNodeGetParams)(
CUgraphNode, 
CUDA_MEM_ALLOC_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphMemAllocNodeGetParams(CUgraphNode hNode, CUDA_MEM_ALLOC_NODE_PARAMS *params_out) {
		fprintf(stderr, "===============\ncuGraphMemAllocNodeGetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphMemAllocNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphMemAllocNodeGetParams = (CUresult (*)(
			CUgraphNode, 
			CUDA_MEM_ALLOC_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphMemAllocNodeGetParams");
			fprintf(stderr, "original_cuGraphMemAllocNodeGetParams:%p\n", original_cuGraphMemAllocNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphMemAllocNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphMemAllocNodeGetParams(
		hNode, 
		params_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}