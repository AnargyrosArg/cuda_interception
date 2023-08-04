#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecExternalSemaphoresWaitNodeSetParams)(
CUgraphExec, 
CUgraphNode, 
const CUDA_EXT_SEM_WAIT_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecExternalSemaphoresWaitNodeSetParams(CUgraphExec hGraphExec, CUgraphNode hNode, const CUDA_EXT_SEM_WAIT_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphExecExternalSemaphoresWaitNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecExternalSemaphoresWaitNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecExternalSemaphoresWaitNodeSetParams = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			const CUDA_EXT_SEM_WAIT_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExecExternalSemaphoresWaitNodeSetParams");
			fprintf(stderr, "original_cuGraphExecExternalSemaphoresWaitNodeSetParams:%p\n", original_cuGraphExecExternalSemaphoresWaitNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecExternalSemaphoresWaitNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphExecExternalSemaphoresWaitNodeSetParams(
		hGraphExec, 
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}