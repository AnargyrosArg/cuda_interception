#include <cuda_runtime_api.h>
#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
#include <execinfo.h>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <string>
#include <map>
#include <climits>

cudaError_t (*original_cudaGetExportTable)(
const void **, 
const cudaUUID_t *
);


extern "C"
{
	cudaError_t cudaGetExportTable(const void **ppExportTable, const cudaUUID_t *pExportTableId) {
		//cudaGetExportTable handwritten as wrapper for cuGetExportTable
		
		fprintf(stderr, "===============\ncudaGetExportTable()\n");
		
		CUresult retval = cuGetExportTable(ppExportTable,(CUuuid*) pExportTableId);
		
		fprintf(stderr, "===============\n");
		
		return (cudaError_t) retval;
	}
}

