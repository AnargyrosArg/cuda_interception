#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuParamSetv)(
CUfunction, 
int, 
void *, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuParamSetv(CUfunction hfunc, int offset, void *ptr, unsigned int numbytes) {
		fprintf(stderr, "===============\ncuParamSetv()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuParamSetv)
		{
			//fetch the original function addr using dlsym
			original_cuParamSetv = (CUresult (*)(
			CUfunction, 
			int, 
			void *, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuParamSetv");
			fprintf(stderr, "original_cuParamSetv:%p\n", original_cuParamSetv);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuParamSetv():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuParamSetv(
		hfunc, 
		offset, 
		ptr, 
		numbytes
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}