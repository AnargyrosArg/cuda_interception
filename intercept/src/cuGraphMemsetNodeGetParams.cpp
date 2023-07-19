#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphMemsetNodeGetParams)(
CUgraphNode, 
CUDA_MEMSET_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphMemsetNodeGetParams(CUgraphNode hNode, CUDA_MEMSET_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphMemsetNodeGetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphMemsetNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphMemsetNodeGetParams = (CUresult (*)(
			CUgraphNode, 
			CUDA_MEMSET_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphMemsetNodeGetParams");
			fprintf(stderr, "original_cuGraphMemsetNodeGetParams:%p\n", original_cuGraphMemsetNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphMemsetNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphMemsetNodeGetParams(
		hNode, 
		nodeParams
		);
	}
}