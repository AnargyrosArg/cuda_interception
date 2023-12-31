#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphMemcpyNodeSetParams)(
CUgraphNode, 
const CUDA_MEMCPY3D *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphMemcpyNodeSetParams(CUgraphNode hNode, const CUDA_MEMCPY3D *nodeParams) {
		fprintf(stderr, "===============\ncuGraphMemcpyNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphMemcpyNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphMemcpyNodeSetParams = (CUresult (*)(
			CUgraphNode, 
			const CUDA_MEMCPY3D *)
			) dlsym(original_libcuda_handle, "cuGraphMemcpyNodeSetParams");
			fprintf(stderr, "original_cuGraphMemcpyNodeSetParams:%p\n", original_cuGraphMemcpyNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphMemcpyNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphMemcpyNodeSetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}