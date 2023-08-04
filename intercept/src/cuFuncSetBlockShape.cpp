#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuFuncSetBlockShape)(
CUfunction, 
int, 
int, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuFuncSetBlockShape(CUfunction hfunc, int x, int y, int z) {
		fprintf(stderr, "===============\ncuFuncSetBlockShape()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuFuncSetBlockShape)
		{
			//fetch the original function addr using dlsym
			original_cuFuncSetBlockShape = (CUresult (*)(
			CUfunction, 
			int, 
			int, 
			int)
			) dlsym(original_libcuda_handle, "cuFuncSetBlockShape");
			fprintf(stderr, "original_cuFuncSetBlockShape:%p\n", original_cuFuncSetBlockShape);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuFuncSetBlockShape():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuFuncSetBlockShape(
		hfunc, 
		x, 
		y, 
		z
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}