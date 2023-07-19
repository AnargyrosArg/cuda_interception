#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuImportExternalSemaphore)(
CUexternalSemaphore *, 
const CUDA_EXTERNAL_SEMAPHORE_HANDLE_DESC *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuImportExternalSemaphore(CUexternalSemaphore *extSem_out, const CUDA_EXTERNAL_SEMAPHORE_HANDLE_DESC *semHandleDesc) {
		fprintf(stderr, "cuImportExternalSemaphore()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuImportExternalSemaphore)
		{
			//fetch the original function addr using dlsym
			original_cuImportExternalSemaphore = (CUresult (*)(
			CUexternalSemaphore *, 
			const CUDA_EXTERNAL_SEMAPHORE_HANDLE_DESC *)
			) dlsym(original_libcuda_handle, "cuImportExternalSemaphore");
			fprintf(stderr, "original_cuImportExternalSemaphore:%p\n", original_cuImportExternalSemaphore);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuImportExternalSemaphore():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuImportExternalSemaphore(
		extSem_out, 
		semHandleDesc
		);
	}
}