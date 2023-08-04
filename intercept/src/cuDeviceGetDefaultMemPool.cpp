#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetDefaultMemPool)(
CUmemoryPool *, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetDefaultMemPool(CUmemoryPool *pool_out, CUdevice dev) {
		fprintf(stderr, "===============\ncuDeviceGetDefaultMemPool()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetDefaultMemPool)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetDefaultMemPool = (CUresult (*)(
			CUmemoryPool *, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuDeviceGetDefaultMemPool");
			fprintf(stderr, "original_cuDeviceGetDefaultMemPool:%p\n", original_cuDeviceGetDefaultMemPool);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetDefaultMemPool():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceGetDefaultMemPool(
		pool_out, 
		dev
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}