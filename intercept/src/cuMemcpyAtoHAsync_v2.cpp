#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyAtoHAsync_v2)(
void *, 
CUarray, 
size_t, 
size_t, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyAtoHAsync_v2(void *dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount, CUstream hStream) {
		fprintf(stderr, "===============\ncuMemcpyAtoHAsync_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyAtoHAsync_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyAtoHAsync_v2 = (CUresult (*)(
			void *, 
			CUarray, 
			size_t, 
			size_t, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemcpyAtoHAsync_v2");
			fprintf(stderr, "original_cuMemcpyAtoHAsync_v2:%p\n", original_cuMemcpyAtoHAsync_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyAtoHAsync_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemcpyAtoHAsync_v2(
		dstHost, 
		srcArray, 
		srcOffset, 
		ByteCount, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}