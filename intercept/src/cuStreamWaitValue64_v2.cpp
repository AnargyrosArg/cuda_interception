#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamWaitValue64_v2)(
CUstream, 
CUdeviceptr, 
cuuint64_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamWaitValue64_v2(CUstream stream, CUdeviceptr addr, cuuint64_t value, unsigned int flags) {
		fprintf(stderr, "===============\ncuStreamWaitValue64_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamWaitValue64_v2)
		{
			//fetch the original function addr using dlsym
			original_cuStreamWaitValue64_v2 = (CUresult (*)(
			CUstream, 
			CUdeviceptr, 
			cuuint64_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamWaitValue64_v2");
			fprintf(stderr, "original_cuStreamWaitValue64_v2:%p\n", original_cuStreamWaitValue64_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamWaitValue64_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamWaitValue64_v2(
		stream, 
		addr, 
		value, 
		flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}