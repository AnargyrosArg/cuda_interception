#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuTexObjectCreate)(
CUtexObject *, 
const CUDA_RESOURCE_DESC *, 
const CUDA_TEXTURE_DESC *, 
const CUDA_RESOURCE_VIEW_DESC *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuTexObjectCreate(CUtexObject *pTexObject, const CUDA_RESOURCE_DESC *pResDesc, const CUDA_TEXTURE_DESC *pTexDesc, const CUDA_RESOURCE_VIEW_DESC *pResViewDesc) {
		fprintf(stderr, "cuTexObjectCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuTexObjectCreate)
		{
			//fetch the original function addr using dlsym
			original_cuTexObjectCreate = (CUresult (*)(
			CUtexObject *, 
			const CUDA_RESOURCE_DESC *, 
			const CUDA_TEXTURE_DESC *, 
			const CUDA_RESOURCE_VIEW_DESC *)
			) dlsym(original_libcuda_handle, "cuTexObjectCreate");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuTexObjectCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuTexObjectCreate(
		pTexObject, 
		pResDesc, 
		pTexDesc, 
		pResViewDesc
		);
	}
}