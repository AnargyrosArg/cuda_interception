#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuModuleUnload)(
CUmodule
);


#define F_TABLE_SIZE 5120


//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern int CALL_COUNTERS[];
extern "C"
{
	CUresult cuModuleUnload(CUmodule hmod) {
		fprintf(stderr, "===============\ncuModuleUnload()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuModuleUnload)
		{
			//fetch the original function addr using dlsym
			original_cuModuleUnload = (CUresult (*)(
			CUmodule)
			) dlsym(original_libcuda_handle, "cuModuleUnload");
			fprintf(stderr, "original_cuModuleUnload:%p\n", original_cuModuleUnload);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuModuleUnload():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuModuleUnload(
		hmod
		);

		fprintf(stderr,"Counters:\n");
		for(int i=0;i<F_TABLE_SIZE;i++){
			if( CALL_COUNTERS[i] !=0 ){
				fprintf(stderr,"trampoline<%d>:called %d times\n",i,CALL_COUNTERS[i]);
			}
		}

		return retval;
	}
}