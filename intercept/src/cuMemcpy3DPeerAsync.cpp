#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpy3DPeerAsync)(
const CUDA_MEMCPY3D_PEER *, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpy3DPeerAsync(const CUDA_MEMCPY3D_PEER *pCopy, CUstream hStream) {
		fprintf(stderr, "cuMemcpy3DPeerAsync()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpy3DPeerAsync)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpy3DPeerAsync = (CUresult (*)(
			const CUDA_MEMCPY3D_PEER *, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuMemcpy3DPeerAsync");
			fprintf(stderr, "original_cuMemcpy3DPeerAsync:%p\n", original_cuMemcpy3DPeerAsync);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpy3DPeerAsync():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemcpy3DPeerAsync(
		pCopy, 
		hStream
		);
	}
}