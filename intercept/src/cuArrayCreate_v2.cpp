#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuArrayCreate_v2)(
CUarray *, 
const CUDA_ARRAY_DESCRIPTOR *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuArrayCreate_v2(CUarray *pHandle, const CUDA_ARRAY_DESCRIPTOR *pAllocateArray) {
		fprintf(stderr, "===============\ncuArrayCreate_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuArrayCreate_v2)
		{
			//fetch the original function addr using dlsym
			original_cuArrayCreate_v2 = (CUresult (*)(
			CUarray *, 
			const CUDA_ARRAY_DESCRIPTOR *)
			) dlsym(original_libcuda_handle, "cuArrayCreate_v2");
			fprintf(stderr, "original_cuArrayCreate_v2:%p\n", original_cuArrayCreate_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuArrayCreate_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuArrayCreate_v2(
		pHandle, 
		pAllocateArray
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}