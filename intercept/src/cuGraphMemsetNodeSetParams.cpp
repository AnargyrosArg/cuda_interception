#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphMemsetNodeSetParams)(
CUgraphNode, 
const CUDA_MEMSET_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphMemsetNodeSetParams(CUgraphNode hNode, const CUDA_MEMSET_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphMemsetNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphMemsetNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphMemsetNodeSetParams = (CUresult (*)(
			CUgraphNode, 
			const CUDA_MEMSET_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphMemsetNodeSetParams");
			fprintf(stderr, "original_cuGraphMemsetNodeSetParams:%p\n", original_cuGraphMemsetNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphMemsetNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphMemsetNodeSetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}