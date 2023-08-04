#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecMemcpyNodeSetParams)(
CUgraphExec, 
CUgraphNode, 
const CUDA_MEMCPY3D *, 
CUcontext
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecMemcpyNodeSetParams(CUgraphExec hGraphExec, CUgraphNode hNode, const CUDA_MEMCPY3D *copyParams, CUcontext ctx) {
		fprintf(stderr, "===============\ncuGraphExecMemcpyNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecMemcpyNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecMemcpyNodeSetParams = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			const CUDA_MEMCPY3D *, 
			CUcontext)
			) dlsym(original_libcuda_handle, "cuGraphExecMemcpyNodeSetParams");
			fprintf(stderr, "original_cuGraphExecMemcpyNodeSetParams:%p\n", original_cuGraphExecMemcpyNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecMemcpyNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphExecMemcpyNodeSetParams(
		hGraphExec, 
		hNode, 
		copyParams, 
		ctx
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}