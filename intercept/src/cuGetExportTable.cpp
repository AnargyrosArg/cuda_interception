#include <cuda.h>
#include <dlfcn.h>
#include <iostream>
#include <execinfo.h>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>

#define F_TABLE_SIZE 5120



void print_backtrace(void);
void dump_export_table(const void** ppExportTable,int entries);
void export_table_mprotect(const void* addr,int size,int FLAGS);
void patch_fake_table(const void* addr,long entries);

CUresult (*original_cuGetExportTable)(const void **, const CUuuid *);

template<int OFFSET> void trampoline();

//fake function table
extern const void*  FAKE_EXPORT_TABLE[];

//copy of real export table
const void* __attribute__((used)) F_TABLE[F_TABLE_SIZE];

//call counters for each function
int  __attribute__((used)) CALL_COUNTERS[F_TABLE_SIZE];





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
		dump_export_table(ppExportTable,16);

		char uuid1[38];
		sprintf(uuid1,"%p %p",*pExportTableId,*(pExportTableId+8));
		fprintf(stderr,"UUID:%s\n",uuid1);
		
		static int ONCE=0;
		if(ONCE==0){
			char uuid[38];
			sprintf(uuid,"%p %p",*pExportTableId,*(pExportTableId+8));
			long long base = 0x6c80;
			long long end = 0x90c0;
			int margin = 1024*sizeof(void*); 
			long offset = 0;

			if(strcmp(uuid,"0x4b70e6bd552008d4 0xf2e1663c12ba348d")==0){
				offset = 0;

			}else if(strcmp(uuid,"0x4ae7f45b6cfbd56b 0xf99dfd1239d98789")==0){
				offset = 0x6ca0 - base; 

			}else if(strcmp(uuid,"0x11df21116e3393c6 0x9395d855f368c3a8")==0){
				offset = 0x6d00 - base; 

			}else if(strcmp(uuid,"0x47cbf623815ad842 0xdcec3a8ae7f69882")==0){
				offset = 0x6f40 - base; 

			}else if(strcmp(uuid,"0x742e742e8c7994a0 0x660a0c200008f293")==0){
				offset = 0x8480 - base; 

			}else if(strcmp(uuid,"0x48321497608c3121 0xf2c82473ff41a68c")==0){
				offset = 0x90c0 - base; 
			}else{
				fprintf(stderr,"Unknown export table!\n");
			}
			//make backup of real export table(s)		
			memcpy(F_TABLE,(*ppExportTable)-offset,end-base + 8*sizeof(void*));
			
			//copy non-pointer values to FAKE_EXPORT_TABLE
			patch_fake_table((*ppExportTable)-offset,F_TABLE_SIZE);

			//make export table writable
			export_table_mprotect((*ppExportTable)-offset,F_TABLE_SIZE*sizeof(void*),PROT_READ | PROT_WRITE);

			//replace export table with fake
			memcpy((void*)(*ppExportTable)-offset,FAKE_EXPORT_TABLE, end-base + 8*sizeof(void*));

			//restore readonly property
			export_table_mprotect((*ppExportTable)-offset,F_TABLE_SIZE*sizeof(void*),PROT_READ);

			//fprintf(stderr,"funcs replaced : %d\n",(end-base)/sizeof(void*));


			ONCE=1;
		}

		dump_export_table(ppExportTable,16);
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


void export_table_mprotect(const void* addr,int size,int FLAGS){
	const int PAGESIZE = getpagesize();
	void* current_page = (void*)(PAGESIZE * ( (long long)addr / (long long)PAGESIZE));

	while( current_page <= addr+size){
		int errorn = mprotect(current_page , PAGESIZE ,FLAGS);
		if(errorn == -1){
			perror("mprotect");
			exit(-1);
		}
		current_page = current_page + PAGESIZE;
	}
	return;
}

void patch_fake_table(const void* addr,long entries){
	for(void* i = (void*)addr ; i < addr+entries*sizeof(void*) ; i=i+sizeof(void*)){
		if( (*(void**)i) < (void*)0x7f0000000000){
			long index = (long)(i - (long)addr)/sizeof(void*);
//			fprintf(stderr,"replacing value %p with -> %p at index %d\n",FAKE_EXPORT_TABLE[index],F_TABLE[index],index);
			FAKE_EXPORT_TABLE[index] = F_TABLE[index];
		}
	}
	return;
}