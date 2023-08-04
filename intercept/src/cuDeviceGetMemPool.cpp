#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetMemPool)(
CUmemoryPool *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetMemPool(CUmemoryPool *pool, CUdevice dev) {
		fprintf(stderr, "===============\ncuDeviceGetMemPool()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetMemPool)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetMemPool = (CUresult (*)(
			CUmemoryPool *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetMemPool");
			fprintf(stderr, "original_cuDeviceGetMemPool:%p\n", original_cuDeviceGetMemPool);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetMemPool():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceGetMemPool(
		pool, 
		dev
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}