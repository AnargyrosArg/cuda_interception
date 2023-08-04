#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExternalSemaphoresWaitNodeSetParams)(
CUgraphNode, 
const CUDA_EXT_SEM_WAIT_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExternalSemaphoresWaitNodeSetParams(CUgraphNode hNode, const CUDA_EXT_SEM_WAIT_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphExternalSemaphoresWaitNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExternalSemaphoresWaitNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExternalSemaphoresWaitNodeSetParams = (CUresult (*)(
			CUgraphNode, 
			const CUDA_EXT_SEM_WAIT_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExternalSemaphoresWaitNodeSetParams");
			fprintf(stderr, "original_cuGraphExternalSemaphoresWaitNodeSetParams:%p\n", original_cuGraphExternalSemaphoresWaitNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExternalSemaphoresWaitNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphExternalSemaphoresWaitNodeSetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}