#ifndef ATOM_H_
#define ATOM_H_

extern int Atom_length(const char * str);

extern const char * Atom_new(const char * str, int len);

extern const char * Atom_string(const char * str);

extern const char * Atom_int(long n);

extern int cmpatom(const void * x, const void * y);

extern unsigned  hashatom(const void * key);

extern int str_cmp(const char * str1, const char * str2);

extern int str_hash(const char * str);

extern int int_cmp(const int * num1, const int * num2);

extern int int_hash(const int * num);

#endif