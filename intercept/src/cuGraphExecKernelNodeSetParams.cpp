#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecKernelNodeSetParams)(
CUgraphExec, 
CUgraphNode, 
const CUDA_KERNEL_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecKernelNodeSetParams(CUgraphExec hGraphExec, CUgraphNode hNode, const CUDA_KERNEL_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphExecKernelNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecKernelNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecKernelNodeSetParams = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			const CUDA_KERNEL_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExecKernelNodeSetParams");
			fprintf(stderr, "original_cuGraphExecKernelNodeSetParams:%p\n", original_cuGraphExecKernelNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecKernelNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphExecKernelNodeSetParams(
		hGraphExec, 
		hNode, 
		nodeParams
		);
	}
}