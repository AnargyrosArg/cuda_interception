#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamCopyAttributes)(
CUstream, 
CUstream
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamCopyAttributes(CUstream dst, CUstream src) {
		fprintf(stderr, "cuStreamCopyAttributes()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamCopyAttributes)
		{
			//fetch the original function addr using dlsym
			original_cuStreamCopyAttributes = (CUresult (*)(
			CUstream, 
			CUstream)
			) dlsym(original_libcuda_handle, "cuStreamCopyAttributes");
			fprintf(stderr, "original_cuStreamCopyAttributes:%p\n", original_cuStreamCopyAttributes);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamCopyAttributes():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamCopyAttributes(
		dst, 
		src
		);
	}
}