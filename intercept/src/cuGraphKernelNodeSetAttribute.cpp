#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphKernelNodeSetAttribute)(
CUgraphNode, 
CUkernelNodeAttrID, 
const CUkernelNodeAttrValue *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphKernelNodeSetAttribute(CUgraphNode hNode, CUkernelNodeAttrID attr, const CUkernelNodeAttrValue *value) {
		fprintf(stderr, "===============\ncuGraphKernelNodeSetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphKernelNodeSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuGraphKernelNodeSetAttribute = (CUresult (*)(
			CUgraphNode, 
			CUkernelNodeAttrID, 
			const CUkernelNodeAttrValue *)
			) dlsym(original_libcuda_handle, "cuGraphKernelNodeSetAttribute");
			fprintf(stderr, "original_cuGraphKernelNodeSetAttribute:%p\n", original_cuGraphKernelNodeSetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphKernelNodeSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphKernelNodeSetAttribute(
		hNode, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}