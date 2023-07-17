#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuImportExternalMemory)(
CUexternalMemory *, 
const CUDA_EXTERNAL_MEMORY_HANDLE_DESC *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuImportExternalMemory(CUexternalMemory *extMem_out, const CUDA_EXTERNAL_MEMORY_HANDLE_DESC *memHandleDesc) {
		fprintf(stderr, "cuImportExternalMemory()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuImportExternalMemory)
		{
			//fetch the original function addr using dlsym
			original_cuImportExternalMemory = (CUresult (*)(
			CUexternalMemory *, 
			const CUDA_EXTERNAL_MEMORY_HANDLE_DESC *)
			) dlsym(original_libcuda_handle, "cuImportExternalMemory");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuImportExternalMemory():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuImportExternalMemory(
		extMem_out, 
		memHandleDesc
		);
	}
}