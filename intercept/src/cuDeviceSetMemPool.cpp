#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceSetMemPool)(
CUdevice, 
CUmemoryPool
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceSetMemPool(CUdevice dev, CUmemoryPool pool) {
		fprintf(stderr, "cuDeviceSetMemPool()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceSetMemPool)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceSetMemPool = (CUresult (*)(
			CUdevice, 
			CUmemoryPool)
			) dlsym(original_libcuda_handle, "cuDeviceSetMemPool");
			fprintf(stderr, "original_cuDeviceSetMemPool:%p\n", original_cuDeviceSetMemPool);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceSetMemPool():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuDeviceSetMemPool(
		dev, 
		pool
		);
	}
}