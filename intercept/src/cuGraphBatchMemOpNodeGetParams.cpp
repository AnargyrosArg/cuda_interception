#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphBatchMemOpNodeGetParams)(
CUgraphNode, 
CUDA_BATCH_MEM_OP_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphBatchMemOpNodeGetParams(CUgraphNode hNode, CUDA_BATCH_MEM_OP_NODE_PARAMS *nodeParams_out) {
		fprintf(stderr, "cuGraphBatchMemOpNodeGetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphBatchMemOpNodeGetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphBatchMemOpNodeGetParams = (CUresult (*)(
			CUgraphNode, 
			CUDA_BATCH_MEM_OP_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphBatchMemOpNodeGetParams");
			fprintf(stderr, "original_cuGraphBatchMemOpNodeGetParams:%p\n", original_cuGraphBatchMemOpNodeGetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphBatchMemOpNodeGetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphBatchMemOpNodeGetParams(
		hNode, 
		nodeParams_out
		);
	}
}