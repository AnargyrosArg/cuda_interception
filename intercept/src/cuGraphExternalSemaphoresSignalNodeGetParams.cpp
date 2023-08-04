#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExternalSemaphoresSignalNodeGetParams)(
CUgraphNode, 
CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExternalSemaphoresSignalNodeGetParams(CUgraphNode hNode, CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *params_out) {
		fprintf(stderr, "===============\ncuGraphExternalSemaphoresSignalNodeGetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExternalSemaphoresSignalNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExternalSemaphoresSignalNodeGetParams = (CUresult (*)(
			CUgraphNode, 
			CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExternalSemaphoresSignalNodeGetParams");
			fprintf(stderr, "original_cuGraphExternalSemaphoresSignalNodeGetParams:%p\n", original_cuGraphExternalSemaphoresSignalNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExternalSemaphoresSignalNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphExternalSemaphoresSignalNodeGetParams(
		hNode, 
		params_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}