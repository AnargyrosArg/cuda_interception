#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddExternalSemaphoresSignalNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddExternalSemaphoresSignalNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *nodeParams) {
		fprintf(stderr, "cuGraphAddExternalSemaphoresSignalNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddExternalSemaphoresSignalNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddExternalSemaphoresSignalNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			const CUDA_EXT_SEM_SIGNAL_NODE_PARAMS *)
			) dlsym(original_libcuda_handle, "cuGraphAddExternalSemaphoresSignalNode");
			fprintf(stderr, "original_cuGraphAddExternalSemaphoresSignalNode:%p\n", original_cuGraphAddExternalSemaphoresSignalNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddExternalSemaphoresSignalNode():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGraphAddExternalSemaphoresSignalNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		nodeParams
		);
	}
}