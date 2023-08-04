#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddMemcpyNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
const CUDA_MEMCPY3D *, 
CUcontext
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddMemcpyNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, const CUDA_MEMCPY3D *copyParams, CUcontext ctx) {
		fprintf(stderr, "===============\ncuGraphAddMemcpyNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddMemcpyNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddMemcpyNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			const CUDA_MEMCPY3D *, 
			CUcontext)
			) dlsym(original_libcuda_handle, "cuGraphAddMemcpyNode");
			fprintf(stderr, "original_cuGraphAddMemcpyNode:%p\n", original_cuGraphAddMemcpyNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddMemcpyNode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphAddMemcpyNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		copyParams, 
		ctx
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}