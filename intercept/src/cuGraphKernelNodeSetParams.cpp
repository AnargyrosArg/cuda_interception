#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphKernelNodeSetParams)(
CUgraphNode, 
const CUDA_KERNEL_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphKernelNodeSetParams(CUgraphNode hNode, const CUDA_KERNEL_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphKernelNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphKernelNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphKernelNodeSetParams = (CUresult (*)(
			CUgraphNode, 
			const CUDA_KERNEL_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphKernelNodeSetParams");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphKernelNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphKernelNodeSetParams(
		hNode, 
		nodeParams
		);
	}
}