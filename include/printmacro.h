#define PRINT(function) \
    fprintf(stderr,"===== "#function" =====\n");\
    function;\
    fprintf(stderr,"=================================\n");
