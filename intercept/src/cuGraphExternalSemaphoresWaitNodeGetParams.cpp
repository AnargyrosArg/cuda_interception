#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExternalSemaphoresWaitNodeGetParams)(
CUgraphNode, 
CUDA_EXT_SEM_WAIT_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExternalSemaphoresWaitNodeGetParams(CUgraphNode hNode, CUDA_EXT_SEM_WAIT_NODE_PARAMS *params_out) {
		fprintf(stderr, "cuGraphExternalSemaphoresWaitNodeGetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExternalSemaphoresWaitNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExternalSemaphoresWaitNodeGetParams = (CUresult (*)(
			CUgraphNode, 
			CUDA_EXT_SEM_WAIT_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExternalSemaphoresWaitNodeGetParams");
			fprintf(stderr, "original_cuGraphExternalSemaphoresWaitNodeGetParams:%p\n", original_cuGraphExternalSemaphoresWaitNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExternalSemaphoresWaitNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphExternalSemaphoresWaitNodeGetParams(
		hNode, 
		params_out
		);
	}
}