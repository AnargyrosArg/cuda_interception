#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemHostAlloc)(
void **, 
size_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemHostAlloc(void **pp, size_t bytesize, unsigned int Flags) {
		fprintf(stderr, "cuMemHostAlloc()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemHostAlloc)
		{
			//fetch the original function addr using dlsym
			original_cuMemHostAlloc = (CUresult (*)(
			void **, 
			size_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuMemHostAlloc");
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemHostAlloc():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemHostAlloc(
		pp, 
		bytesize, 
		Flags
		);
	}
}