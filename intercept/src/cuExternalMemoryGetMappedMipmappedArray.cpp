#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuExternalMemoryGetMappedMipmappedArray)(
CUmipmappedArray *, 
CUexternalMemory, 
const CUDA_EXTERNAL_MEMORY_MIPMAPPED_ARRAY_DESC *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuExternalMemoryGetMappedMipmappedArray(CUmipmappedArray *mipmap, CUexternalMemory extMem, const CUDA_EXTERNAL_MEMORY_MIPMAPPED_ARRAY_DESC *mipmapDesc) {
		fprintf(stderr, "cuExternalMemoryGetMappedMipmappedArray()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuExternalMemoryGetMappedMipmappedArray)
		{
			//fetch the original function addr using dlsym
			original_cuExternalMemoryGetMappedMipmappedArray = (CUresult (*)(
			CUmipmappedArray *, 
			CUexternalMemory, 
			const CUDA_EXTERNAL_MEMORY_MIPMAPPED_ARRAY_DESC *)
			) dlsym(original_libcuda_handle, "cuExternalMemoryGetMappedMipmappedArray");
			fprintf(stderr, "original_cuExternalMemoryGetMappedMipmappedArray:%p\n", original_cuExternalMemoryGetMappedMipmappedArray);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuExternalMemoryGetMappedMipmappedArray():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuExternalMemoryGetMappedMipmappedArray(
		mipmap, 
		extMem, 
		mipmapDesc
		);
	}
}