#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphHostNodeGetParams)(
CUgraphNode, 
CUDA_HOST_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphHostNodeGetParams(CUgraphNode hNode, CUDA_HOST_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphHostNodeGetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphHostNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphHostNodeGetParams = (CUresult (*)(
			CUgraphNode, 
			CUDA_HOST_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphHostNodeGetParams");
			fprintf(stderr, "original_cuGraphHostNodeGetParams:%p\n", original_cuGraphHostNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphHostNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphHostNodeGetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}