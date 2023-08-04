#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamSetAttribute)(
CUstream, 
CUstreamAttrID, 
const CUstreamAttrValue *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamSetAttribute(CUstream hStream, CUstreamAttrID attr, const CUstreamAttrValue *value) {
		fprintf(stderr, "===============\ncuStreamSetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuStreamSetAttribute = (CUresult (*)(
			CUstream, 
			CUstreamAttrID, 
			const CUstreamAttrValue *)
			) dlsym(original_libcuda_handle, "cuStreamSetAttribute");
			fprintf(stderr, "original_cuStreamSetAttribute:%p\n", original_cuStreamSetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamSetAttribute(
		hStream, 
		attr, 
		value
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}