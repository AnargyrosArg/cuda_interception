#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecHostNodeSetParams)(
CUgraphExec, 
CUgraphNode, 
const CUDA_HOST_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecHostNodeSetParams(CUgraphExec hGraphExec, CUgraphNode hNode, const CUDA_HOST_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphExecHostNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecHostNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecHostNodeSetParams = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			const CUDA_HOST_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExecHostNodeSetParams");
			fprintf(stderr, "original_cuGraphExecHostNodeSetParams:%p\n", original_cuGraphExecHostNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecHostNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphExecHostNodeSetParams(
		hGraphExec, 
		hNode, 
		nodeParams
		);
	}
}