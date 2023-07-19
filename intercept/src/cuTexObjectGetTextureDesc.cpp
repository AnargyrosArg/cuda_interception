#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexObjectGetTextureDesc)(
CUDA_TEXTURE_DESC *, 
CUtexObject
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexObjectGetTextureDesc(CUDA_TEXTURE_DESC *pTexDesc, CUtexObject texObject) {
		fprintf(stderr, "cuTexObjectGetTextureDesc()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexObjectGetTextureDesc)
		{
			//fetch the original function addr using dlsym
			original_cuTexObjectGetTextureDesc = (CUresult (*)(
			CUDA_TEXTURE_DESC *, 
			CUtexObject)
			) dlsym(original_libcuda_handle, "cuTexObjectGetTextureDesc");
			fprintf(stderr, "original_cuTexObjectGetTextureDesc:%p\n", original_cuTexObjectGetTextureDesc);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexObjectGetTextureDesc():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexObjectGetTextureDesc(
		pTexDesc, 
		texObject
		);
	}
}