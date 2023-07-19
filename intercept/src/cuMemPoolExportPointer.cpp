#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolExportPointer)(
CUmemPoolPtrExportData *, 
CUdeviceptr
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolExportPointer(CUmemPoolPtrExportData *shareData_out, CUdeviceptr ptr) {
		fprintf(stderr, "cuMemPoolExportPointer()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolExportPointer)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolExportPointer = (CUresult (*)(
			CUmemPoolPtrExportData *, 
			CUdeviceptr)
			) dlsym(original_libcuda_handle, "cuMemPoolExportPointer");
			fprintf(stderr, "original_cuMemPoolExportPointer:%p\n", original_cuMemPoolExportPointer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolExportPointer():%s\n", __dlerror);
			fflush(stderr);
		}
		return original_cuMemPoolExportPointer(
		shareData_out, 
		ptr
		);
	}
}