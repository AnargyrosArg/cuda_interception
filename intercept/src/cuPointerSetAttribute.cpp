#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuPointerSetAttribute)(
const void *, 
CUpointer_attribute, 
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuPointerSetAttribute(const void *value, CUpointer_attribute attribute, CUdeviceptr ptr) {
		fprintf(stderr, "===============\ncuPointerSetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuPointerSetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuPointerSetAttribute = (CUresult (*)(
			const void *, 
			CUpointer_attribute, 
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuPointerSetAttribute");
			fprintf(stderr, "original_cuPointerSetAttribute:%p\n", original_cuPointerSetAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuPointerSetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuPointerSetAttribute(
		value, 
		attribute, 
		ptr
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}