#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPrefetchAsync)(
CUdeviceptr, 
size_t, 
CUdevice, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPrefetchAsync(CUdeviceptr devPtr, size_t count, CUdevice dstDevice, CUstream hStream) {
		fprintf(stderr, "cuMemPrefetchAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPrefetchAsync)
		{
			//fetch the original function addr using dlsym
			original_cuMemPrefetchAsync = (CUresult (*)(
			CUdeviceptr, 
			size_t, 
			CUdevice, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemPrefetchAsync");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPrefetchAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemPrefetchAsync(
		devPtr, 
		count, 
		dstDevice, 
		hStream
		);
	}
}