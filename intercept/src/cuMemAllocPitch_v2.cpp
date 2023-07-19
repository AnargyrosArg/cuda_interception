#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemAllocPitch_v2)(
CUdeviceptr *, 
size_t *, 
size_t, 
size_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemAllocPitch_v2(CUdeviceptr *dptr, size_t *pPitch, size_t WidthInBytes, size_t Height, unsigned int ElementSizeBytes) {
		fprintf(stderr, "cuMemAllocPitch_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemAllocPitch_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemAllocPitch_v2 = (CUresult (*)(
			CUdeviceptr *, 
			size_t *, 
			size_t, 
			size_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuMemAllocPitch_v2");
			fprintf(stderr, "original_cuMemAllocPitch_v2:%p\n", original_cuMemAllocPitch_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemAllocPitch_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemAllocPitch_v2(
		dptr, 
		pPitch, 
		WidthInBytes, 
		Height, 
		ElementSizeBytes
		);
	}
}