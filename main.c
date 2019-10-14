
#include <stdio.h>
#include <stdlib.h>
#include "atom.h"
#include "except.h"
#include "assert.h"

int main(int argc, char * argv[]) {
    TRY
        printf("main() run() begin..\n");

        const char * a = Atom_string("12314");
        const char * b = Atom_int(12314);
        printf("first atom is '%s' and it's length is :%d\n", a, Atom_length(a));
        printf("second atom is '%s' and it's length is :%d\n", b, Atom_length(b));

    
        const char * c = Atom_string(NULL);
        printf("third atom is '%s' and it's length is :%d\n", c, Atom_length(c));
    EXCEPT(Assert_Failed)
        printf("\n-----------------------------------------\n"
            "catch excepting,and message is : '%s'\n", Assert_Failed.reason);
        printf("call Atom_string() fail,because param is NULL.\n");
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