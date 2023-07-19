#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphExecBatchMemOpNodeSetParams)(
CUgraphExec, 
CUgraphNode, 
const CUDA_BATCH_MEM_OP_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphExecBatchMemOpNodeSetParams(CUgraphExec hGraphExec, CUgraphNode hNode, const CUDA_BATCH_MEM_OP_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphExecBatchMemOpNodeSetParams()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphExecBatchMemOpNodeSetParams)
		{
			//fetch the original function addr using dlsym
			original_cuGraphExecBatchMemOpNodeSetParams = (CUresult (*)(
			CUgraphExec, 
			CUgraphNode, 
			const CUDA_BATCH_MEM_OP_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphExecBatchMemOpNodeSetParams");
			fprintf(stderr, "original_cuGraphExecBatchMemOpNodeSetParams:%p\n", original_cuGraphExecBatchMemOpNodeSetParams);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphExecBatchMemOpNodeSetParams():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphExecBatchMemOpNodeSetParams(
		hGraphExec, 
		hNode, 
		nodeParams
		);
	}
}