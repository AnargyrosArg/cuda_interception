#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuArrayGetMemoryRequirements)(
CUDA_ARRAY_MEMORY_REQUIREMENTS *, 
CUarray, 
CUdevice
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuArrayGetMemoryRequirements(CUDA_ARRAY_MEMORY_REQUIREMENTS *memoryRequirements, CUarray array, CUdevice device) {
		fprintf(stderr, "cuArrayGetMemoryRequirements()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuArrayGetMemoryRequirements)
		{
			//fetch the original function addr using dlsym
			original_cuArrayGetMemoryRequirements = (CUresult (*)(
			CUDA_ARRAY_MEMORY_REQUIREMENTS *, 
			CUarray, 
			CUdevice)
			) dlsym(original_libcuda_handle, "cuArrayGetMemoryRequirements");
			fprintf(stderr, "original_cuArrayGetMemoryRequirements:%p\n", original_cuArrayGetMemoryRequirements);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuArrayGetMemoryRequirements():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuArrayGetMemoryRequirements(
		memoryRequirements, 
		array, 
		device
		);
	}
}