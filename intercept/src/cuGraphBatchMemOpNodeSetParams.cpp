#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphBatchMemOpNodeSetParams)(
CUgraphNode, 
const CUDA_BATCH_MEM_OP_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphBatchMemOpNodeSetParams(CUgraphNode hNode, const CUDA_BATCH_MEM_OP_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "===============\ncuGraphBatchMemOpNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphBatchMemOpNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphBatchMemOpNodeSetParams = (CUresult (*)(
			CUgraphNode, 
			const CUDA_BATCH_MEM_OP_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphBatchMemOpNodeSetParams");
			fprintf(stderr, "original_cuGraphBatchMemOpNodeSetParams:%p\n", original_cuGraphBatchMemOpNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphBatchMemOpNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphBatchMemOpNodeSetParams(
		hNode, 
		nodeParams
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}