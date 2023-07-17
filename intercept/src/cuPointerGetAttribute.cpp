#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuPointerGetAttribute)(
void *, 
CUpointer_attribute, 
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuPointerGetAttribute(void *data, CUpointer_attribute attribute, CUdeviceptr ptr) {
		fprintf(stderr, "cuPointerGetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuPointerGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuPointerGetAttribute = (CUresult (*)(
			void *, 
			CUpointer_attribute, 
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuPointerGetAttribute");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuPointerGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuPointerGetAttribute(
		data, 
		attribute, 
		ptr
		);
	}
}