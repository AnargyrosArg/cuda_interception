#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamBatchMemOp)(
CUstream, 
unsigned int, 
CUstreamBatchMemOpParams *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamBatchMemOp(CUstream stream, unsigned int count, CUstreamBatchMemOpParams *paramArray, unsigned int flags) {
		fprintf(stderr, "cuStreamBatchMemOp()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamBatchMemOp)
		{
			//fetch the original function addr using dlsym
			original_cuStreamBatchMemOp = (CUresult (*)(
			CUstream, 
			unsigned int, 
			CUstreamBatchMemOpParams *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuStreamBatchMemOp");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamBatchMemOp():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamBatchMemOp(
		stream, 
		count, 
		paramArray, 
		flags
		);
	}
}