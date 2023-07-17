#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamWaitValue64)(
CUstream, 
CUdeviceptr, 
cuuint64_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamWaitValue64(CUstream stream, CUdeviceptr addr, cuuint64_t value, unsigned int flags) {
		fprintf(stderr, "cuStreamWaitValue64()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamWaitValue64)
		{
			//fetch the original function addr using dlsym
			original_cuStreamWaitValue64 = (CUresult (*)(
			CUstream, 
			CUdeviceptr, 
			cuuint64_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamWaitValue64");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamWaitValue64():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamWaitValue64(
		stream, 
		addr, 
		value, 
		flags
		);
	}
}