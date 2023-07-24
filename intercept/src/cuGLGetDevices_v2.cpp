#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGLGetDevices_v2)(
unsigned int *, 
CUdevice *, 
unsigned int, 
CUGLDeviceList
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGLGetDevices_v2(unsigned int *pCudaDeviceCount, CUdevice *pCudaDevices, unsigned int cudaDeviceCount, CUGLDeviceList deviceList) {
		fprintf(stderr, "cuGLGetDevices_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGLGetDevices_v2)
		{
			//fetch the original function addr using dlsym
			original_cuGLGetDevices_v2 = (CUresult (*)(
			unsigned int *, 
			CUdevice *, 
			unsigned int, 
			CUGLDeviceList)
			) dlsym(original_libcuda_handle, "cuGLGetDevices_v2");
			fprintf(stderr, "original_cuGLGetDevices_v2:%p\n", original_cuGLGetDevices_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGLGetDevices_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGLGetDevices_v2(
		pCudaDeviceCount, 
		pCudaDevices, 
		cudaDeviceCount, 
		deviceList
		);
	}
}