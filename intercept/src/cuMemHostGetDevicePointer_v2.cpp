#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemHostGetDevicePointer_v2)(
CUdeviceptr *, 
void *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemHostGetDevicePointer_v2(CUdeviceptr *pdptr, void *p, unsigned int Flags) {
		fprintf(stderr, "===============\ncuMemHostGetDevicePointer_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemHostGetDevicePointer_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemHostGetDevicePointer_v2 = (CUresult (*)(
			CUdeviceptr *, 
			void *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuMemHostGetDevicePointer_v2");
			fprintf(stderr, "original_cuMemHostGetDevicePointer_v2:%p\n", original_cuMemHostGetDevicePointer_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemHostGetDevicePointer_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemHostGetDevicePointer_v2(
		pdptr, 
		p, 
		Flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}