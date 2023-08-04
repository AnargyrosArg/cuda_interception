#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemcpy3DPeer)(
const CUDA_MEMCPY3D_PEER *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemcpy3DPeer(const CUDA_MEMCPY3D_PEER *pCopy) {
		fprintf(stderr, "===============\ncuMemcpy3DPeer()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemcpy3DPeer)
		{
			//fetch the original function addr using dlsym
			original_cuMemcpy3DPeer = (CUresult (*)(
			const CUDA_MEMCPY3D_PEER *)
			) dlsym(original_libcuda_handle, "cuMemcpy3DPeer");
			fprintf(stderr, "original_cuMemcpy3DPeer:%p\n", original_cuMemcpy3DPeer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemcpy3DPeer():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemcpy3DPeer(
		pCopy
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}