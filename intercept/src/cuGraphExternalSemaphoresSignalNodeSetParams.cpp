#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExternalSemaphoresSignalNodeSetParams)(
CUgraphNode, 
const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExternalSemaphoresSignalNodeSetParams(CUgraphNode hNode, const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphExternalSemaphoresSignalNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExternalSemaphoresSignalNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExternalSemaphoresSignalNodeSetParams = (CUresult (*)(
			CUgraphNode, 
			const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExternalSemaphoresSignalNodeSetParams");
			fprintf(stderr, "original_cuGraphExternalSemaphoresSignalNodeSetParams:%p\n", original_cuGraphExternalSemaphoresSignalNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExternalSemaphoresSignalNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphExternalSemaphoresSignalNodeSetParams(
		hNode, 
		nodeParams
		);
	}
}