#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
#include <execinfo.h>


void* fn_ptr0;
void* fn_ptr1;
void* fn_ptr2;


int dummy0(void){
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = 0; i < frames; ++i) {
	    printf("%s\n", strs[i]);
	}
	free(strs);
	fprintf(stderr,"dummy0 called\n");
	return 1;
}
int dummy1(void){
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = 0; i < frames; ++i) {
	    printf("%s\n", strs[i]);
	}
	free(strs);
	fprintf(stderr,"dummy1 called\n");
	return 1;
}
int dummy2(void){
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = 0; i < frames; ++i) {
	    printf("%s\n", strs[i]);
	}
	free(strs);
	fprintf(stderr,"dummy2 called\n");
	return 2 << 20;
}
typedef int (*ExportedFunction)();
static ExportedFunction exportTable[3] = {&dummy0, &dummy1, &dummy1};

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
			original_cuGetExportTable = (CUresult (*)(const void **, const CUuuid *)) dlsym(original_libcuda_handle, "cuGetExportTable");
			fprintf(stderr, "original_cuGetExportTable:%p\n", original_cuGetExportTable);
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGetExportTable():%s\n", __dlerror);
			fflush(stderr);
		}
		
		CUresult result =  original_cuGetExportTable(ppExportTable, pExportTableId);
		



		//*ppExportTable = &exportTable;

		return result;
	}
}