#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecMemsetNodeSetParams)(
CUgraphExec, 
CUgraphNode, 
const CUDA_MEMSET_NODE_PARAMS *, 
CUcontext
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecMemsetNodeSetParams(CUgraphExec hGraphExec, CUgraphNode hNode, const CUDA_MEMSET_NODE_PARAMS *memsetParams, CUcontext ctx) {
		fprintf(stderr, "cuGraphExecMemsetNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecMemsetNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecMemsetNodeSetParams = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			const CUDA_MEMSET_NODE_PARAMS *, 
			CUcontext)
			) dlsym(original_libcuda_handle, "cuGraphExecMemsetNodeSetParams");
			fprintf(stderr, "original_cuGraphExecMemsetNodeSetParams:%p\n", original_cuGraphExecMemsetNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecMemsetNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphExecMemsetNodeSetParams(
		hGraphExec, 
		hNode, 
		memsetParams, 
		ctx
		);
	}
}