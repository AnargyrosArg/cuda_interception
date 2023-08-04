#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuDeviceGetNvSciSyncAttributes)(
void *, 
CUdevice, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuDeviceGetNvSciSyncAttributes(void *nvSciSyncAttrList, CUdevice dev, int flags) {
		fprintf(stderr, "===============\ncuDeviceGetNvSciSyncAttributes()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuDeviceGetNvSciSyncAttributes)
		{
			//fetch the original function addr using dlsym
			original_cuDeviceGetNvSciSyncAttributes = (CUresult (*)(
			void *, 
			CUdevice, 
			int)
			) dlsym(original_libcuda_handle, "cuDeviceGetNvSciSyncAttributes");
			fprintf(stderr, "original_cuDeviceGetNvSciSyncAttributes:%p\n", original_cuDeviceGetNvSciSyncAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuDeviceGetNvSciSyncAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuDeviceGetNvSciSyncAttributes(
		nvSciSyncAttrList, 
		dev, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}