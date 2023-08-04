#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuSurfObjectGetResourceDesc)(
CUDA_RESOURCE_DESC *, 
CUsurfObject
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuSurfObjectGetResourceDesc(CUDA_RESOURCE_DESC *pResDesc, CUsurfObject surfObject) {
		fprintf(stderr, "===============\ncuSurfObjectGetResourceDesc()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuSurfObjectGetResourceDesc)
		{
			//fetch the original function addr using dlsym
			original_cuSurfObjectGetResourceDesc = (CUresult (*)(
			CUDA_RESOURCE_DESC *, 
			CUsurfObject)
			) dlsym(original_libcuda_handle, "cuSurfObjectGetResourceDesc");
			fprintf(stderr, "original_cuSurfObjectGetResourceDesc:%p\n", original_cuSurfObjectGetResourceDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuSurfObjectGetResourceDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuSurfObjectGetResourceDesc(
		pResDesc, 
		surfObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}