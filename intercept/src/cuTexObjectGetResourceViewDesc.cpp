#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexObjectGetResourceViewDesc)(
CUDA_RESOURCE_VIEW_DESC *, 
CUtexObject
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexObjectGetResourceViewDesc(CUDA_RESOURCE_VIEW_DESC *pResViewDesc, CUtexObject texObject) {
		fprintf(stderr, "===============\ncuTexObjectGetResourceViewDesc()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexObjectGetResourceViewDesc)
		{
			//fetch the original function addr using dlsym
			original_cuTexObjectGetResourceViewDesc = (CUresult (*)(
			CUDA_RESOURCE_VIEW_DESC *, 
			CUtexObject)
			) dlsym(original_libcuda_handle, "cuTexObjectGetResourceViewDesc");
			fprintf(stderr, "original_cuTexObjectGetResourceViewDesc:%p\n", original_cuTexObjectGetResourceViewDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexObjectGetResourceViewDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuTexObjectGetResourceViewDesc(
		pResViewDesc, 
		texObject
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}