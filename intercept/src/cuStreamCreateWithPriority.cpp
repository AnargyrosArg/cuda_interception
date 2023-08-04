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
		fprintf(stderr, "===============\ncuStreamCreateWithPriority()\n");
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
			fprintf(stderr, "original_cuStreamCreateWithPriority:%p\n", original_cuStreamCreateWithPriority);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuStreamCreateWithPriority():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuStreamCreateWithPriority(
		phStream, 
		flags, 
		priority
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}