#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemsetD2D8Async)(
CUdeviceptr, 
size_t, 
unsigned char, 
size_t, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemsetD2D8Async(CUdeviceptr dstDevice, size_t dstPitch, unsigned char uc, size_t Width, size_t Height, CUstream hStream) {
		fprintf(stderr, "===============\ncuMemsetD2D8Async()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemsetD2D8Async)
		{
			//fetch the original function addr using dlsym
			original_cuMemsetD2D8Async = (CUresult (*)(
			CUdeviceptr, 
			size_t, 
			unsigned char, 
			size_t, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemsetD2D8Async");
			fprintf(stderr, "original_cuMemsetD2D8Async:%p\n", original_cuMemsetD2D8Async);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemsetD2D8Async():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemsetD2D8Async(
		dstDevice, 
		dstPitch, 
		uc, 
		Width, 
		Height, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}