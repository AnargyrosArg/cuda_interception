#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemRangeGetAttribute)(
void *, 
size_t, 
CUmem_range_attribute, 
CUdeviceptr, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemRangeGetAttribute(void *data, size_t dataSize, CUmem_range_attribute attribute, CUdeviceptr devPtr, size_t count) {
		fprintf(stderr, "cuMemRangeGetAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemRangeGetAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuMemRangeGetAttribute = (CUresult (*)(
			void *, 
			size_t, 
			CUmem_range_attribute, 
			CUdeviceptr, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemRangeGetAttribute");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemRangeGetAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemRangeGetAttribute(
		data, 
		dataSize, 
		attribute, 
		devPtr, 
		count
		);
	}
}