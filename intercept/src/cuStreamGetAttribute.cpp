#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamGetAttribute)(
CUstream, 
CUstreamAttrID, 
CUstreamAttrValue *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamGetAttribute(CUstream hStream, CUstreamAttrID attr, CUstreamAttrValue *value_out) {
		fprintf(stderr, "===============\ncuStreamGetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuStreamGetAttribute = (CUresult (*)(
			CUstream, 
			CUstreamAttrID, 
			CUstreamAttrValue *)
			) dlsym(original_libcuda_handle, "cuStreamGetAttribute");
			fprintf(stderr, "original_cuStreamGetAttribute:%p\n", original_cuStreamGetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamGetAttribute(
		hStream, 
		attr, 
		value_out
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}