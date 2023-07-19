#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamWaitValue32_v2)(
CUstream, 
CUdeviceptr, 
cuuint32_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamWaitValue32_v2(CUstream stream, CUdeviceptr addr, cuuint32_t value, unsigned int flags) {
		fprintf(stderr, "cuStreamWaitValue32_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamWaitValue32_v2)
		{
			//fetch the original function addr using dlsym
			original_cuStreamWaitValue32_v2 = (CUresult (*)(
			CUstream, 
			CUdeviceptr, 
			cuuint32_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamWaitValue32_v2");
			fprintf(stderr, "original_cuStreamWaitValue32_v2:%p\n", original_cuStreamWaitValue32_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamWaitValue32_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamWaitValue32_v2(
		stream, 
		addr, 
		value, 
		flags
		);
	}
}