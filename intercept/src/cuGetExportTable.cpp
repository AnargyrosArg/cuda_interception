#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
#include <execinfo.h>
#include <cstring>
#include <map>
#include <vector>


void print_backtrace(void);
void dump_export_table(const void** ppExportTable,int entries);

CUresult (*original_cuGetExportTable)(const void **, const CUuuid *);

template<int OFFSET> void trampoline();

//fake function table
const void*  FAKE_EXPORT_TABLE[] = {
	(void*)0x58,
	(void*)trampoline<1>,
	(void*)trampoline<2>,
	(void*)trampoline<3>,
	(void*)trampoline<4>,
	(void*)trampoline<5>,
	(void*)trampoline<6>,
	(void*)trampoline<7>,
	(void*)trampoline<8>,
	(void*)trampoline<9>,
	(void*)trampoline<10>,
	(void*)trampoline<11>,
	(void*)trampoline<12>,
	(void*)trampoline<13>,
	(void*)trampoline<14>,
	(void*)trampoline<15>,
	(void*)trampoline<16>,
	(void*)trampoline<17>,
	(void*)trampoline<18>,
	(void*)trampoline<19>,
	(void*)trampoline<20>,
	(void*)trampoline<21>,
	(void*)trampoline<22>,
	(void*)trampoline<23>,
	(void*)trampoline<24>,
	(void*)trampoline<25>,
	(void*)trampoline<26>,
	(void*)trampoline<27>,
	(void*)trampoline<29>,
	(void*)trampoline<30>,
	(void*)trampoline<31>,
	(void*)trampoline<32>,
	(void*)trampoline<33>,
	(void*)trampoline<34>,
	(void*)trampoline<35>,
	(void*)trampoline<36>,
	(void*)trampoline<37>,
	(void*)trampoline<38>,
	(void*)trampoline<39>,
	(void*)trampoline<40>,
	(void*)trampoline<41>,
	(void*)trampoline<42>,
	(void*)trampoline<43>,
	(void*)trampoline<44>,
	(void*)trampoline<45>,
	(void*)trampoline<46>,
	(void*)trampoline<47>,
	(void*)trampoline<48>,
	}; 

//copy of real export table
const void* __attribute__((used)) F_TABLE[128];

//call counters for each function
int  __attribute__((used)) CALL_COUNTERS[128];





template<int OFFSET>
void trampoline(){
    asm ("push %r10");
	asm ("push %r11");
	//increment counter
    
	//get address of CALL_COUNTERS from Global Offset Table
	asm volatile("movq CALL_COUNTERS@GOTPCREL(%rip), %r10");
	//select index we want

	asm ("add %0, %%r10"
        :
        :"i" (OFFSET*4)
        :
        );

	//dereference pointer to get value
    asm volatile("mov (%r10), %r11");
	//increment
    asm volatile("add $0x01 , %r11"); 
    //write back
	asm volatile("mov %r11,(%r10)");

    //call actual function
    
	//get address of F_TABLE from Global Offset Table
	asm volatile("movq F_TABLE@GOTPCREL(%rip), %r10");
    
	//select index
	asm ("add %0, %%r10"
        :
        :"i" (OFFSET*8)
        :
        );

	//get address of function
    asm volatile("mov (%r10), %r8");
	
	asm("pop %r10");
	asm("pop %r11");

	//jump to function
   	asm volatile("jmp *%r8");
}



//handle to the actual libcuda library, used to fetch original functions with dlsym
extern void* original_libcuda_handle;
extern "C"
{
	CUresult cuGetExportTable(const void **ppExportTable, const CUuuid *pExportTableId) {
		fprintf(stderr, "cuGetExportTable()\n");
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
		}
		__dlerror = dlerror();
		if(__dlerror){
			fprintf(stderr, "dlsym error for function cuGetExportTable():%s\n", __dlerror);
			fflush(stderr);
		}
		
		CUresult result =  original_cuGetExportTable(ppExportTable, pExportTableId);	
		

		fprintf(stderr,"%d: *pExportTableId: %p %p\n",result,*pExportTableId,*(pExportTableId+8));
		char uuid1[38];
		sprintf(uuid1,"%p %p",*pExportTableId,*(pExportTableId+8));
		fprintf(stderr,"UUID:%s\n",uuid1);

		if(strcmp(uuid1,"0x4ae7f45b6cfbd56b 0xf99dfd1239d98789")==0){
			fprintf(stderr,"Swapping tables for %s\n",uuid1);
			memcpy(F_TABLE,*ppExportTable,128*sizeof(void*));
			dump_export_table(ppExportTable,35);
			*ppExportTable = FAKE_EXPORT_TABLE;
			fprintf(stderr,"===================================\n");
			for(int i=0;i<35;i++){
				fprintf(stderr,"%d:%p\n",i,F_TABLE[i]);
			}
		}

		for(int i=0;i<20;i++){
			fprintf(stderr,"func%d:%d\n",i,CALL_COUNTERS[i]);
		}
		return result;
	}
}



//utility functions
void print_backtrace(void){
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);
	for (i = 1; i < frames; ++i) {
	    fprintf(stderr,"%s\n", strs[i]);
	}
	free(strs);
}

void dump_export_table(const void** ppExportTable,int entries){
	for(void* i = (void*)*ppExportTable ; i < *ppExportTable+entries*sizeof(void*) ; i=i+sizeof(void*)){
		fprintf(stderr,"%d -> %p\n", (long)(i - (long)*ppExportTable)/sizeof(void*)  , *(void**)i);
	}
	return;
}
