#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemGetHandleForAddressRange)(
void *, 
CUdeviceptr, 
size_t, 
CUmemRangeHandleType, 
unsigned long long
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemGetHandleForAddressRange(void *handle, CUdeviceptr dptr, size_t size, CUmemRangeHandleType handleType, unsigned long long flags) {
		fprintf(stderr, "===============\ncuMemGetHandleForAddressRange()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemGetHandleForAddressRange)
		{
			//fetch the original function addr using dlsym
			original_cuMemGetHandleForAddressRange = (CUresult (*)(
			void *, 
			CUdeviceptr, 
			size_t, 
			CUmemRangeHandleType, 
			unsigned long long)
			) dlsym(original_libcuda_handle, "cuMemGetHandleForAddressRange");
			fprintf(stderr, "original_cuMemGetHandleForAddressRange:%p\n", original_cuMemGetHandleForAddressRange);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemGetHandleForAddressRange():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemGetHandleForAddressRange(
		handle, 
		dptr, 
		size, 
		handleType, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}