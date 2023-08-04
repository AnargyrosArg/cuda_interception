#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemGetAllocationGranularity)(
size_t *, 
const CUmemAllocationProp *, 
CUmemAllocationGranularity_flags
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemGetAllocationGranularity(size_t *granularity, const CUmemAllocationProp *prop, CUmemAllocationGranularity_flags option) {
		fprintf(stderr, "===============\ncuMemGetAllocationGranularity()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemGetAllocationGranularity)
		{
			//fetch the original function addr using dlsym
			original_cuMemGetAllocationGranularity = (CUresult (*)(
			size_t *, 
			const CUmemAllocationProp *, 
			CUmemAllocationGranularity_flags)
			) dlsym(original_libcuda_handle, "cuMemGetAllocationGranularity");
			fprintf(stderr, "original_cuMemGetAllocationGranularity:%p\n", original_cuMemGetAllocationGranularity);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemGetAllocationGranularity():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemGetAllocationGranularity(
		granularity, 
		prop, 
		option
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}