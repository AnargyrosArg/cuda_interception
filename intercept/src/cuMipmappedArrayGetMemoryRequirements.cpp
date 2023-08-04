#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMipmappedArrayGetMemoryRequirements)(
CUDA_ARRAY_MEMORY_REQUIREMENTS *, 
CUmipmappedArray, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMipmappedArrayGetMemoryRequirements(CUDA_ARRAY_MEMORY_REQUIREMENTS *memoryRequirements, CUmipmappedArray mipmap, CUdevice device) {
		fprintf(stderr, "===============\ncuMipmappedArrayGetMemoryRequirements()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMipmappedArrayGetMemoryRequirements)
		{
			//fetch the original function addr using dlsym
			original_cuMipmappedArrayGetMemoryRequirements = (CUresult (*)(
			CUDA_ARRAY_MEMORY_REQUIREMENTS *, 
			CUmipmappedArray, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuMipmappedArrayGetMemoryRequirements");
			fprintf(stderr, "original_cuMipmappedArrayGetMemoryRequirements:%p\n", original_cuMipmappedArrayGetMemoryRequirements);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMipmappedArrayGetMemoryRequirements():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMipmappedArrayGetMemoryRequirements(
		memoryRequirements, 
		mipmap, 
		device
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}