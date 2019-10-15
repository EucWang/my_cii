#include <stdio.h>
#include "examples.h"
#include "mem.h"

void test_atom() {

	const char* a = Atom_string("12314");
	printf("first atom is '%s' and it's length is :%d\n", a, Atom_length(a));
	printf("----------------------------------------\n");
	const char* b = Atom_int(12314);
	printf("second atom is '%s' and it's length is :%d\n", b, Atom_length(b));
	printf("----------------------------------------\n");

	const char* d =Atom_string("sahdfkret2q4656o6yu,mfdgaetrertei7tyjhw534uyt");
	printf("second atom is '%s' and it's length is :%d\n", d, Atom_length(d));
	printf("----------------------------------------\n");

	const char* c = Atom_string(NULL);
	printf("third atom is '%s' and it's length is :%d\n", c, Atom_length(c));
	printf("----------------------------------------\n");
}