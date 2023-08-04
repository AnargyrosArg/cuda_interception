#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceCanAccessPeer)(
int *, 
CUdevice, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceCanAccessPeer(int *canAccessPeer, CUdevice dev, CUdevice peerDev) {
		fprintf(stderr, "===============\ncuDeviceCanAccessPeer()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceCanAccessPeer)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceCanAccessPeer = (CUresult (*)(
			int *, 
			CUdevice, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceCanAccessPeer");
			fprintf(stderr, "original_cuDeviceCanAccessPeer:%p\n", original_cuDeviceCanAccessPeer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceCanAccessPeer():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceCanAccessPeer(
		canAccessPeer, 
		dev, 
		peerDev
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}