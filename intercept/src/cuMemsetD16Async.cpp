#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemsetD16Async)(
CUdeviceptr, 
unsigned short, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemsetD16Async(CUdeviceptr dstDevice, unsigned short us, size_t N, CUstream hStream) {
		fprintf(stderr, "cuMemsetD16Async()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemsetD16Async)
		{
			//fetch the original function addr using dlsym
			original_cuMemsetD16Async = (CUresult (*)(
			CUdeviceptr, 
			unsigned short, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemsetD16Async");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemsetD16Async():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemsetD16Async(
		dstDevice, 
		us, 
		N, 
		hStream
		);
	}
}