#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetTexture1DLinearMaxWidth)(
size_t *, 
CUarray_format, 
unsigned, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetTexture1DLinearMaxWidth(size_t *maxWidthInElements, CUarray_format format, unsigned numChannels, CUdevice dev) {
		fprintf(stderr, "cuDeviceGetTexture1DLinearMaxWidth()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetTexture1DLinearMaxWidth)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetTexture1DLinearMaxWidth = (CUresult (*)(
			size_t *, 
			CUarray_format, 
			unsigned, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetTexture1DLinearMaxWidth");
			fprintf(stderr, "original_cuDeviceGetTexture1DLinearMaxWidth:%p\n", original_cuDeviceGetTexture1DLinearMaxWidth);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetTexture1DLinearMaxWidth():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetTexture1DLinearMaxWidth(
		maxWidthInElements, 
		format, 
		numChannels, 
		dev
		);
	}
}