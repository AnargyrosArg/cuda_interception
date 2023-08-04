#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuExternalMemoryGetMappedBuffer)(
CUdeviceptr *, 
CUexternalMemory, 
const CUDA_EXTERNAL_MEMORY_BUFFER_DESC *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuExternalMemoryGetMappedBuffer(CUdeviceptr *devPtr, CUexternalMemory extMem, const CUDA_EXTERNAL_MEMORY_BUFFER_DESC *bufferDesc) {
		fprintf(stderr, "===============\ncuExternalMemoryGetMappedBuffer()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuExternalMemoryGetMappedBuffer)
		{
			//fetch the original function addr using dlsym
			original_cuExternalMemoryGetMappedBuffer = (CUresult (*)(
			CUdeviceptr *, 
			CUexternalMemory, 
			const CUDA_EXTERNAL_MEMORY_BUFFER_DESC *)
			) dlsym(original_libcuda_handle, "cuExternalMemoryGetMappedBuffer");
			fprintf(stderr, "original_cuExternalMemoryGetMappedBuffer:%p\n", original_cuExternalMemoryGetMappedBuffer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuExternalMemoryGetMappedBuffer():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuExternalMemoryGetMappedBuffer(
		devPtr, 
		extMem, 
		bufferDesc
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}