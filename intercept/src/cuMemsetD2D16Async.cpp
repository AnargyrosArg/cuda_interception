#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemsetD2D16Async)(
CUdeviceptr, 
size_t, 
unsigned short, 
size_t, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemsetD2D16Async(CUdeviceptr dstDevice, size_t dstPitch, unsigned short us, size_t Width, size_t Height, CUstream hStream) {
		fprintf(stderr, "cuMemsetD2D16Async()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemsetD2D16Async)
		{
			//fetch the original function addr using dlsym
			original_cuMemsetD2D16Async = (CUresult (*)(
			CUdeviceptr, 
			size_t, 
			unsigned short, 
			size_t, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemsetD2D16Async");
			fprintf(stderr, "original_cuMemsetD2D16Async:%p\n", original_cuMemsetD2D16Async);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemsetD2D16Async():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemsetD2D16Async(
		dstDevice, 
		dstPitch, 
		us, 
		Width, 
		Height, 
		hStream
		);
	}
}