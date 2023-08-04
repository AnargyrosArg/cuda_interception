#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpyAtoH_v2)(
void *, 
CUarray, 
size_t, 
size_t
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpyAtoH_v2(void *dstHost, CUarray srcArray, size_t srcOffset, size_t ByteCount) {
		fprintf(stderr, "===============\ncuMemcpyAtoH_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpyAtoH_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpyAtoH_v2 = (CUresult (*)(
			void *, 
			CUarray, 
			size_t, 
			size_t)
			) dlsym(original_libcuda_handle, "cuMemcpyAtoH_v2");
			fprintf(stderr, "original_cuMemcpyAtoH_v2:%p\n", original_cuMemcpyAtoH_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpyAtoH_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemcpyAtoH_v2(
		dstHost, 
		srcArray, 
		srcOffset, 
		ByteCount
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}