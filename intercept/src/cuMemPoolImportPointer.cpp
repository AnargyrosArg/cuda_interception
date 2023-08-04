#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuMemPoolImportPointer)(
CUdeviceptr *, 
CUmemoryPool, 
CUmemPoolPtrExportData *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuMemPoolImportPointer(CUdeviceptr *ptr_out, CUmemoryPool pool, CUmemPoolPtrExportData *shareData) {
		fprintf(stderr, "===============\ncuMemPoolImportPointer()\n");
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuMemPoolImportPointer)
		{
			//fetch the original function addr using dlsym
			original_cuMemPoolImportPointer = (CUresult (*)(
			CUdeviceptr *, 
			CUmemoryPool, 
			CUmemPoolPtrExportData *)
			) dlsym(original_libcuda_handle, "cuMemPoolImportPointer");
			fprintf(stderr, "original_cuMemPoolImportPointer:%p\n", original_cuMemPoolImportPointer);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuMemPoolImportPointer():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult retval = original_cuMemPoolImportPointer(
		ptr_out, 
		pool, 
		shareData
		);
		fprintf(stderr, "===============\n");
		return retval;
	}
}