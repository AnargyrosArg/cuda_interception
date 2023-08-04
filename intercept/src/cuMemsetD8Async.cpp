#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemsetD8Async)(
CUdeviceptr, 
unsigned char, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemsetD8Async(CUdeviceptr dstDevice, unsigned char uc, size_t N, CUstream hStream) {
		fprintf(stderr, "===============\ncuMemsetD8Async()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemsetD8Async)
		{
			//fetch the original function addr using dlsym
			original_cuMemsetD8Async = (CUresult (*)(
			CUdeviceptr, 
			unsigned char, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemsetD8Async");
			fprintf(stderr, "original_cuMemsetD8Async:%p\n", original_cuMemsetD8Async);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemsetD8Async():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemsetD8Async(
		dstDevice, 
		uc, 
		N, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}