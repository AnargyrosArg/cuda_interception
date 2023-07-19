#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMipmappedArrayCreate)(
CUmipmappedArray *, 
const CUDA_ARRAY3D_DESCRIPTOR *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMipmappedArrayCreate(CUmipmappedArray *pHandle, const CUDA_ARRAY3D_DESCRIPTOR *pMipmappedArrayDesc, unsigned int numMipmapLevels) {
		fprintf(stderr, "cuMipmappedArrayCreate()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMipmappedArrayCreate)
		{
			//fetch the original function addr using dlsym
			original_cuMipmappedArrayCreate = (CUresult (*)(
			CUmipmappedArray *, 
			const CUDA_ARRAY3D_DESCRIPTOR *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuMipmappedArrayCreate");
			fprintf(stderr, "original_cuMipmappedArrayCreate:%p\n", original_cuMipmappedArrayCreate);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMipmappedArrayCreate():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMipmappedArrayCreate(
		pHandle, 
		pMipmappedArrayDesc, 
		numMipmapLevels
		);
	}
}