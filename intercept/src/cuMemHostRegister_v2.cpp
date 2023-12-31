#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemHostRegister_v2)(
void *, 
size_t, 
unsigned int
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemHostRegister_v2(void *p, size_t bytesize, unsigned int Flags) {
		fprintf(stderr, "===============\ncuMemHostRegister_v2()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemHostRegister_v2)
		{
			//fetch the original function addr using dlsym
			original_cuMemHostRegister_v2 = (CUresult (*)(
			void *, 
			size_t, 
			unsigned int)
			) dlsym(original_libcuda_handle, "cuMemHostRegister_v2");
			fprintf(stderr, "original_cuMemHostRegister_v2:%p\n", original_cuMemHostRegister_v2);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemHostRegister_v2():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemHostRegister_v2(
		p, 
		bytesize, 
		Flags
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}