#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyHtoA_v2)(
CUarray, 
size_t, 
const void *, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyHtoA_v2(CUarray dstArray, size_t dstOffset, const void *srcHost, size_t ByteCount) {
		fprintf(stderr, "===============\ncuMemcpyHtoA_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyHtoA_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyHtoA_v2 = (CUresult (*)(
			CUarray, 
			size_t, 
			const void *, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemcpyHtoA_v2");
			fprintf(stderr, "original_cuMemcpyHtoA_v2:%p\n", original_cuMemcpyHtoA_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyHtoA_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemcpyHtoA_v2(
		dstArray, 
		dstOffset, 
		srcHost, 
		ByteCount
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}