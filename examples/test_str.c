/**
 * Created by wangxn on 2019/12/20.
*/

#include "examples.h"
#include "str.h"
#include "fmt.h"

void test_str(){
    Fmt_print("||||||||test_str() running.\n");

    char * str = "when I was young, I listen to the radio, waiting for my favorite song.";

    char *substr = Str_substr(str, 1, 5);

    Fmt_print("'%s'\n", str);
    Fmt_print("'%s'\n", substr);
    Fmt_print("'%s'\n", Str_duple_substr(str, 8,11, 3));

    char * str2 = "hello darkness my old friend, I've never come to you again.";

    Fmt_print("'%s'\n", Str_concat(str, 1, 5, str2, 7, 15));

    Fmt_print("||||||||test_str() done.\n");
}