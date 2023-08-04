#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphKernelNodeGetParams)(
CUgraphNode, 
CUDA_KERNEL_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphKernelNodeGetParams(CUgraphNode hNode, CUDA_KERNEL_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphKernelNodeGetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphKernelNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphKernelNodeGetParams = (CUresult (*)(
			CUgraphNode, 
			CUDA_KERNEL_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphKernelNodeGetParams");
			fprintf(stderr, "original_cuGraphKernelNodeGetParams:%p\n", original_cuGraphKernelNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphKernelNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphKernelNodeGetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}