#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphHostNodeSetParams)(
CUgraphNode, 
const CUDA_HOST_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphHostNodeSetParams(CUgraphNode hNode, const CUDA_HOST_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphHostNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphHostNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphHostNodeSetParams = (CUresult (*)(
			CUgraphNode, 
			const CUDA_HOST_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphHostNodeSetParams");
			fprintf(stderr, "original_cuGraphHostNodeSetParams:%p\n", original_cuGraphHostNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphHostNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphHostNodeSetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}