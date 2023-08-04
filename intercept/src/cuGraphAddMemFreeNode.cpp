#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphAddMemFreeNode)(
CUgraphNode *, 
CUgraph, 
const CUgraphNode *, 
size_t, 
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphAddMemFreeNode(CUgraphNode *phGraphNode, CUgraph hGraph, const CUgraphNode *dependencies, size_t numDependencies, CUdeviceptr dptr) {
		fprintf(stderr, "===============\ncuGraphAddMemFreeNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphAddMemFreeNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphAddMemFreeNode = (CUresult (*)(
			CUgraphNode *, 
			CUgraph, 
			const CUgraphNode *, 
			size_t, 
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuGraphAddMemFreeNode");
			fprintf(stderr, "original_cuGraphAddMemFreeNode:%p\n", original_cuGraphAddMemFreeNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphAddMemFreeNode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphAddMemFreeNode(
		phGraphNode, 
		hGraph, 
		dependencies, 
		numDependencies, 
		dptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}