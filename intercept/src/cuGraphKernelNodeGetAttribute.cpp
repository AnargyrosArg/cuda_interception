#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGraphKernelNodeGetAttribute)(
CUgraphNode, 
CUkernelNodeAttrID, 
CUkernelNodeAttrValue *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGraphKernelNodeGetAttribute(CUgraphNode hNode, CUkernelNodeAttrID attr, CUkernelNodeAttrValue *value_out) {
		fprintf(stderr, "===============\ncuGraphKernelNodeGetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGraphKernelNodeGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuGraphKernelNodeGetAttribute = (CUresult (*)(
			CUgraphNode, 
			CUkernelNodeAttrID, 
			CUkernelNodeAttrValue *)
			) dlsym(original_libcuda_handle, "cuGraphKernelNodeGetAttribute");
			fprintf(stderr, "original_cuGraphKernelNodeGetAttribute:%p\n", original_cuGraphKernelNodeGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGraphKernelNodeGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuGraphKernelNodeGetAttribute(
		hNode, 
		attr, 
		value_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}