#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemGetAllocationPropertiesFromHandle)(
CUmemAllocationProp *, 
CUmemGenericAllocationHandle
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemGetAllocationPropertiesFromHandle(CUmemAllocationProp *prop, CUmemGenericAllocationHandle handle) {
		fprintf(stderr, "cuMemGetAllocationPropertiesFromHandle()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemGetAllocationPropertiesFromHandle)
		{
			//fetch the original function addr using dlsym
			original_cuMemGetAllocationPropertiesFromHandle = (CUresult (*)(
			CUmemAllocationProp *, 
			CUmemGenericAllocationHandle)
			) dlsym(original_libcuda_handle, "cuMemGetAllocationPropertiesFromHandle");
			fprintf(stderr, "original_cuMemGetAllocationPropertiesFromHandle:%p\n", original_cuMemGetAllocationPropertiesFromHandle);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemGetAllocationPropertiesFromHandle():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemGetAllocationPropertiesFromHandle(
		prop, 
		handle
		);
	}
}