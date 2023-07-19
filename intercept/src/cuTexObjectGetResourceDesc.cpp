#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexObjectGetResourceDesc)(
CUDA_RESOURCE_DESC *, 
CUtexObject
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexObjectGetResourceDesc(CUDA_RESOURCE_DESC *pResDesc, CUtexObject texObject) {
		fprintf(stderr, "cuTexObjectGetResourceDesc()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexObjectGetResourceDesc)
		{
			//fetch the original function addr using dlsym
			original_cuTexObjectGetResourceDesc = (CUresult (*)(
			CUDA_RESOURCE_DESC *, 
			CUtexObject)
			) dlsym(original_libcuda_handle, "cuTexObjectGetResourceDesc");
			fprintf(stderr, "original_cuTexObjectGetResourceDesc:%p\n", original_cuTexObjectGetResourceDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexObjectGetResourceDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexObjectGetResourceDesc(
		pResDesc, 
		texObject
		);
	}
}