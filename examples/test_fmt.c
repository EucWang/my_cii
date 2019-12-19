/**
 * Created by wangxn on 2019/12/19.
*/

#include "examples.h"
#include "fmt.h"
#include <string.h>
#include <stdio.h>

void test_fmt3(){
    Fmt_print("test_fmt3() running.\n-------------------\n");

    //对fputc进行函数签名转换是必须的, 原本的fputc的类型是 : int (*)(int , FILE*)
    Fmt_fmt((int (*)(int, void *))fputc, stdout, "wa ha ha %2d , %s, %e\n", 321123, "didadida di ", 546.323423);
    char * name = "Array";
    int count = 8;
    Fmt_fmt((int (*)(int, void *))fputc, stdout, "The %s interface has %d functions\n", name, count);

    Fmt_print("test_fmt3() done.\n-------------------\n");
}

void test_fmt2(){
    Fmt_print("test_fmt2() running.\n-------------------\n");
//    Fmt_fmt(putc, "wa ha ha %2d , %s, %e", 321123, "didadida di ", 546.323423);
    char buf[255] = {'\0'};
    int i = Fmt_sfmt(buf, 255, "who you are?\nmy name is %s.\n", "zhang san");
    Fmt_print("i is %d\n", i);


    int age = 15;
    i = Fmt_sfmt((buf + strlen(buf)), 255 - strlen(buf), "and how old are you?\n%d\n", age);
    Fmt_print("i is %d\n", i);

    Fmt_print("%s", buf);
    Fmt_print("test_fmt2() done.\n-------------------\n");
}

void test_ffmt(){
    Fmt_print("test_ffmt() running.\n-------------------\n");
    FILE *fp = NULL;

    fp = fopen("test_fmt2.txt", "w+");
//    fprintf(fp, "This is testing for fprintf...\n");
//    fputs("This is testing for fputs...\n", fp);

    double da = -312434.5;
    char * str3 = "random double is '%10.3e'\n";
    Fmt_fprint(fp, str3, da);
    fclose(fp);
    Fmt_print("test_ffmt() done.\n-------------------\n");
}

void test_fmt1(){
    Fmt_print("test_fmt1() running.\n-------------------\n");

    Fmt_print("\n%s is good man.\n", "zhang san");

    Fmt_print("when i was young, i listen to the radio, waiting for my favorite song.\n");;

    int a = 123;
    char * name = "li si";
    Fmt_print("age is %d, name is %s.\n", a, name);
    Fmt_print("age is '%06d', name is [%10s].\n", a, name);

    char * name2 = "wa ha ha";
    Fmt_print("another name is [%-15s].\n", name2);

    double da = -312434.5;
    char * str3 = "random double is '%10.2e'\n";
    char * str3s = Fmt_string(str3, da);
    Fmt_print("%s", str3s);

    Fmt_print("test_fmt1() done.\n-------------------\n");
}

void test_fmt() {
    printf("test_fmt() runing!\n-------------------\n");
//    test_fmt1();
//    test_ffmt();
//    test_fmt2();
    test_fmt3();

    printf("test_fmt() done!\n-------------------\n");
}
