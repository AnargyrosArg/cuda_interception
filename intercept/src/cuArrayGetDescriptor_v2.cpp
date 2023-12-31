#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuArrayGetDescriptor_v2)(
CUDA_ARRAY_DESCRIPTOR *, 
CUarray
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuArrayGetDescriptor_v2(CUDA_ARRAY_DESCRIPTOR *pArrayDescriptor, CUarray hArray) {
		fprintf(stderr, "===============\ncuArrayGetDescriptor_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuArrayGetDescriptor_v2)
		{
			//fetch the original function addr using dlsym
			original_cuArrayGetDescriptor_v2 = (CUresult (*)(
			CUDA_ARRAY_DESCRIPTOR *, 
			CUarray)
			) dlsym(original_libcuda_handle, "cuArrayGetDescriptor_v2");
			fprintf(stderr, "original_cuArrayGetDescriptor_v2:%p\n", original_cuArrayGetDescriptor_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuArrayGetDescriptor_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuArrayGetDescriptor_v2(
		pArrayDescriptor, 
		hArray
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}