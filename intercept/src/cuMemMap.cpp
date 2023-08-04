#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemMap)(
CUdeviceptr, 
size_t, 
size_t, 
CUmemGenericAllocationHandle, 
unsigned long long
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemMap(CUdeviceptr ptr, size_t size, size_t offset, CUmemGenericAllocationHandle handle, unsigned long long flags) {
		fprintf(stderr, "===============\ncuMemMap()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemMap)
		{
			//fetch the original function addr using dlsym
			original_cuMemMap = (CUresult (*)(
			CUdeviceptr, 
			size_t, 
			size_t, 
			CUmemGenericAllocationHandle, 
			unsigned long long)
			) dlsym(original_libcuda_handle, "cuMemMap");
			fprintf(stderr, "original_cuMemMap:%p\n", original_cuMemMap);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemMap():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemMap(
		ptr, 
		size, 
		offset, 
		handle, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}