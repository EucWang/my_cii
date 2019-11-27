
#include <stdio.h>
#include <stdlib.h>
#include "atom.h"
#include "except.h"
#include "assert.h"
#include "examples.h"
#include "mem.h"

int main(int argc, char * argv[]) {
    TRY
        printf("main() run() begin..\n");
        // test_atom();
//        test_mem();
//		test_arena();
//        test_list();
//        test_getword(argc, argv);
//         test_table();
//        test_set();
//        test_xref(argc, argv);
//        test_array();
//        test_seq();
        test_ring();
	EXCEPT(Assert_Failed)
        printf("\n-----------------------------------------\n"
            "catch excepting,and message is : '%s'\n", Assert_Failed.reason);
        printf("call Atom_string() fail,because param is NULL.\n");
    EXCEPT(Mem_Failed)
        printf("\n-----------------------------------------\n"
            "catch excepting,and message is : '%s'\n", Mem_Failed.reason);
        printf("call test_mem() fail.\n");
    ELSE   //断言失败时,在程序的最顶层代码中放置一个TRY-EXCEPT语句,捕获所有的未捕获的异常,并输出帮助信息,然后再次抛出异常
        fprintf(stderr, "\n-----------------------------------------\n"
            "An internal error has occurred from which there is " 
            "no recovery.\nPlease report this error to "
            "Technical Support at xxx-xxx-xxxx.\nNot the"
            " following message, which will help our support staff"
            "\nfind the cause of this error."
            "\n-----------------------------------------\n");
        RERAISE;
    END_TRY;

    printf("\n---------------------------\nout of try and still runing.Yohoooooooo\n");

    return EXIT_SUCCESS;
}