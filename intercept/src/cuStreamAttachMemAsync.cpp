#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamAttachMemAsync)(
CUstream, 
CUdeviceptr, 
size_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamAttachMemAsync(CUstream hStream, CUdeviceptr dptr, size_t length, unsigned int flags) {
		fprintf(stderr, "===============\ncuStreamAttachMemAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamAttachMemAsync)
		{
			//fetch the original function addr using dlsym
			original_cuStreamAttachMemAsync = (CUresult (*)(
			CUstream, 
			CUdeviceptr, 
			size_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamAttachMemAsync");
			fprintf(stderr, "original_cuStreamAttachMemAsync:%p\n", original_cuStreamAttachMemAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamAttachMemAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamAttachMemAsync(
		hStream, 
		dptr, 
		length, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}