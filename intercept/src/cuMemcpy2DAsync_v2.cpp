#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpy2DAsync_v2)(
const CUDA_MEMCPY2D *, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpy2DAsync_v2(const CUDA_MEMCPY2D *pCopy, CUstream hStream) {
		fprintf(stderr, "===============\ncuMemcpy2DAsync_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpy2DAsync_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpy2DAsync_v2 = (CUresult (*)(
			const CUDA_MEMCPY2D *, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemcpy2DAsync_v2");
			fprintf(stderr, "original_cuMemcpy2DAsync_v2:%p\n", original_cuMemcpy2DAsync_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpy2DAsync_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemcpy2DAsync_v2(
		pCopy, 
		hStream
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}