#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddBatchMemOpNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
const CUDA_BATCH_MEM_OP_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddBatchMemOpNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, const CUDA_BATCH_MEM_OP_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphAddBatchMemOpNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddBatchMemOpNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddBatchMemOpNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			const CUDA_BATCH_MEM_OP_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphAddBatchMemOpNode");
			fprintf(stderr, "original_cuGraphAddBatchMemOpNode:%p\n", original_cuGraphAddBatchMemOpNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddBatchMemOpNode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphAddBatchMemOpNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		nodeParams
		);
	}
}