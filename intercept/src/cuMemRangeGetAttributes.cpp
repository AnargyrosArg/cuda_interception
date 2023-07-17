#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemRangeGetAttributes)(
void **, 
size_t *, 
CUmem_range_attribute *, 
size_t, 
CUdeviceptr, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemRangeGetAttributes(void **data, size_t *dataSizes, CUmem_range_attribute *attributes, size_t numAttributes, CUdeviceptr devPtr, size_t count) {
		fprintf(stderr, "cuMemRangeGetAttributes()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemRangeGetAttributes)
		{
			//fetch the original function addr using dlsym
			original_cuMemRangeGetAttributes = (CUresult (*)(
			void **, 
			size_t *, 
			CUmem_range_attribute *, 
			size_t, 
			CUdeviceptr, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemRangeGetAttributes");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemRangeGetAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemRangeGetAttributes(
		data, 
		dataSizes, 
		attributes, 
		numAttributes, 
		devPtr, 
		count
		);
	}
}