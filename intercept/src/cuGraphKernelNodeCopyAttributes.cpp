#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphKernelNodeCopyAttributes)(
CUgraphNode, 
CUgraphNode
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphKernelNodeCopyAttributes(CUgraphNode dst, CUgraphNode src) {
		fprintf(stderr, "===============\ncuGraphKernelNodeCopyAttributes()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphKernelNodeCopyAttributes)
		{
			//fetch the original function addr using dlsym
			original_cuGraphKernelNodeCopyAttributes = (CUresult (*)(
			CUgraphNode, 
			CUgraphNode)
			) dlsym(original_libcuda_handle, "cuGraphKernelNodeCopyAttributes");
			fprintf(stderr, "original_cuGraphKernelNodeCopyAttributes:%p\n", original_cuGraphKernelNodeCopyAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphKernelNodeCopyAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphKernelNodeCopyAttributes(
		dst, 
		src
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}