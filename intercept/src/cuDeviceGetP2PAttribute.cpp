#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetP2PAttribute)(
int *, 
CUdevice_P2PAttribute, 
CUdevice, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetP2PAttribute(int *value, CUdevice_P2PAttribute attrib, CUdevice srcDevice, CUdevice dstDevice) {
		fprintf(stderr, "cuDeviceGetP2PAttribute()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetP2PAttribute)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetP2PAttribute = (CUresult (*)(
			int *, 
			CUdevice_P2PAttribute, 
			CUdevice, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetP2PAttribute");
			fprintf(stderr, "original_cuDeviceGetP2PAttribute:%p\n", original_cuDeviceGetP2PAttribute);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetP2PAttribute():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceGetP2PAttribute(
		value, 
		attrib, 
		srcDevice, 
		dstDevice
		);
	}
}