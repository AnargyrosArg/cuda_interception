#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemsetD32Async)(
CUdeviceptr, 
unsigned int, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemsetD32Async(CUdeviceptr dstDevice, unsigned int ui, size_t N, CUstream hStream) {
		fprintf(stderr, "cuMemsetD32Async()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemsetD32Async)
		{
			//fetch the original function addr using dlsym
			original_cuMemsetD32Async = (CUresult (*)(
			CUdeviceptr, 
			unsigned int, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemsetD32Async");
			fprintf(stderr, "original_cuMemsetD32Async:%p\n", original_cuMemsetD32Async);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemsetD32Async():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemsetD32Async(
		dstDevice, 
		ui, 
		N, 
		hStream
		);
	}
}