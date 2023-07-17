#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuStreamCreateWithPriority)(
CUstream *, 
unsigned int, 
int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuStreamCreateWithPriority(CUstream *phStream, unsigned int flags, int priority) {
		fprintf(stderr, "cuStreamCreateWithPriority()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuStreamCreateWithPriority)
		{
			//fetch the original function addr using dlsym
			original_cuStreamCreateWithPriority = (CUresult (*)(
			CUstream *, 
			unsigned int, 
			int)
			) dlsym(original_libcuda_handle, "cuStreamCreateWithPriority");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamCreateWithPriority():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuStreamCreateWithPriority(
		phStream, 
		flags, 
		priority
		);
	}
}