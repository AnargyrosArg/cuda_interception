#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemAddressReserve)(
CUdeviceptr *, 
size_t, 
size_t, 
CUdeviceptr, 
unsigned long long
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemAddressReserve(CUdeviceptr *ptr, size_t size, size_t alignment, CUdeviceptr addr, unsigned long long flags) {
		fprintf(stderr, "cuMemAddressReserve()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemAddressReserve)
		{
			//fetch the original function addr using dlsym
			original_cuMemAddressReserve = (CUresult (*)(
			CUdeviceptr *, 
			size_t, 
			size_t, 
			CUdeviceptr, 
			unsigned long long)
			) dlsym(original_libcuda_handle, "cuMemAddressReserve");
			fprintf(stderr, "original_cuMemAddressReserve:%p\n", original_cuMemAddressReserve);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemAddressReserve():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemAddressReserve(
		ptr, 
		size, 
		alignment, 
		addr, 
		flags
		);
	}
}