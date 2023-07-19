#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
CUresult (*original_cuGetExportTable)(
const void **, 
const CUuuid *
);
//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGetExportTable(const void **ppExportTable, const CUuuid *pExportTableId) {
		fprintf(stderr, "cuGetExportTable() UUID:%p\n",*pExportTableId);
		char* __dlerror;
		//this call clears any previous errors
		dlerror();
		//this calls the intercepted dlopen function which initializes the handle to libcuda (original_libcuda_handle)
		if(original_libcuda_handle == NULL){
			dlopen("libcuda.so.1", RTLD_NOW);
		}
		if (!original_cuGetExportTable)
		{
			//fetch the original function addr using dlsym
			original_cuGetExportTable = (CUresult (*)(
			const void **, 
			const CUuuid *)
			) dlsym(original_libcuda_handle, "cuGetExportTable");
			fprintf(stderr, "original_cuGetExportTable:%p\n", original_cuGetExportTable);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGetExportTable():%s\n", __dlerror);
			fflush(stderr);
		}
		CUresult result =  original_cuGetExportTable(
		ppExportTable, 
		pExportTableId
		);


		for(int i=0;i< 5;i++){
			fprintf(stderr,"*ppExportTable[%i]: %p\n",i,*(ppExportTable + sizeof(void*)*i));
		}

		return result;


	}
}