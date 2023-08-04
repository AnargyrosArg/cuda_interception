#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphDestroyNode)(
CUgraphNode
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphDestroyNode(CUgraphNode hNode) {
		fprintf(stderr, "===============\ncuGraphDestroyNode()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphDestroyNode)
		{
			//fetch the original function addr using dlsym
			original_cuGraphDestroyNode = (CUresult (*)(
			CUgraphNode)
			) dlsym(original_libcuda_handle, "cuGraphDestroyNode");
			fprintf(stderr, "original_cuGraphDestroyNode:%p\n", original_cuGraphDestroyNode);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphDestroyNode():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphDestroyNode(
		hNode
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}