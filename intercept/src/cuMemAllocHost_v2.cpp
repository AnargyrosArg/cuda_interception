#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemAllocHost_v2)(
void **, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemAllocHost_v2(void **pp, size_t bytesize) {
		fprintf(stderr, "===============\ncuMemAllocHost_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemAllocHost_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemAllocHost_v2 = (CUresult (*)(
			void **, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemAllocHost_v2");
			fprintf(stderr, "original_cuMemAllocHost_v2:%p\n", original_cuMemAllocHost_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemAllocHost_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemAllocHost_v2(
		pp, 
		bytesize
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}