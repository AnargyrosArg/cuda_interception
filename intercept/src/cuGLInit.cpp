#include <cudaGL.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGLInit)(
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGLInit() {
		fprintf(stderr, "cuGLInit()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGLInit)
		{
			//fetch the original function addr using dlsym
			original_cuGLInit = (CUresult (*)(
			void)
			) dlsym(original_libcuda_handle, "cuGLInit");
			fprintf(stderr, "original_cuGLInit:%p\n", original_cuGLInit);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGLInit():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuGLInit(
		);
	}
}