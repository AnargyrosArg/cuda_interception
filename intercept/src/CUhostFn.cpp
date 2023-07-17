/*#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
void (*original_CUhostFn)(
void *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	// void CUhostFn(void *userData) {
		fprintf(stderr, "CUhostFn()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_CUhostFn)
		{
			//fetch the original function addr using dlsym
			original_CUhostFn = (void (*)(
			void *)
			) dlsym(original_libcuda_handle, "CUhostFn");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function CUhostFn():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_CUhostFn(
		userData
		);
	}
}
*/
