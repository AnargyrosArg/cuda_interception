#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecExternalSemaphoresSignalNodeSetParams)(
CUgraphExec, 
CUgraphNode, 
const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecExternalSemaphoresSignalNodeSetParams(CUgraphExec hGraphExec, CUgraphNode hNode, const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphExecExternalSemaphoresSignalNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecExternalSemaphoresSignalNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecExternalSemaphoresSignalNodeSetParams = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExecExternalSemaphoresSignalNodeSetParams");
			fprintf(stderr, "original_cuGraphExecExternalSemaphoresSignalNodeSetParams:%p\n", original_cuGraphExecExternalSemaphoresSignalNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecExternalSemaphoresSignalNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphExecExternalSemaphoresSignalNodeSetParams(
		hGraphExec, 
		hNode, 
		nodeParams
		);
	}
}