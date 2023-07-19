#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamBatchMemOp_v2)(
CUstream, 
unsigned int, 
CUstreamBatchMemOpParams *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamBatchMemOp_v2(CUstream stream, unsigned int count, CUstreamBatchMemOpParams *paramArray, unsigned int flags) {
		fprintf(stderr, "cuStreamBatchMemOp_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamBatchMemOp_v2)
		{
			//fetch the original function addr using dlsym
			original_cuStreamBatchMemOp_v2 = (CUresult (*)(
			CUstream, 
			unsigned int, 
			CUstreamBatchMemOpParams *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamBatchMemOp_v2");
			fprintf(stderr, "original_cuStreamBatchMemOp_v2:%p\n", original_cuStreamBatchMemOp_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamBatchMemOp_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamBatchMemOp_v2(
		stream, 
		count, 
		paramArray, 
		flags
		);
	}
}