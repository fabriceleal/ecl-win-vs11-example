/*
	Based on http://vwood.github.io/embedded-ecl.html
	so, essentially, copy pasted the code and foo'ed around with 
	project settings until it worked.
	My ECL is installed under C:\ecl\13.5.1\
	This folder must be in the PATH (it needs ecl.dll)

	... and fixed an error (check the function void init())

*/
#include "stdafx.h"
#include <ecl/ecl.h>

#define DEFUN(name,fun,args) \
    cl_def_c_function(c_string_to_object(name), \
                      (cl_objectfn_fixed)fun, \
                      args)
 
cl_object foo() {
    return ecl_make_integer(42);
}
 
cl_object bar(cl_object a, cl_object b) {
    int aval = fix(a);
    int bval = fix(b);
    return ecl_make_integer(aval + bval);
}
 
/*
    Assumes the string is a valid call.
*/
cl_object ecl_call(char *call) {
    return cl_safe_eval(c_string_to_object(call), Cnil, Cnil);
}
 
void init() {
    // For some reason, the original (char **)&"" 
    // breaks the original example :(
    //
    // if you are a C++ wiz, push an explanation ;)
    //
    char** args = (char**) malloc(sizeof(char*) * 1);
    args[0] = "EclEmbeddingTest";
    cl_boot(1, args); 
 
    atexit(cl_shutdown);
 
/* 
    Uncomment these lines to place your code into a separate package,
    They may then be called like (my-code:foo)
 */
//  ecl_call("(make-package :my-code)");
//  ecl_call("(in-package my-code)");
 
    DEFUN("foo", foo, 0);
    DEFUN("bar", bar, 2);
 
//  ecl_call("(export foo)");
//  ecl_call("(export bar)");
//  ecl_call("(in-package common-lisp-user)");
}
 
int main() {
    init();

    cl_object exit_obj = c_string_to_object(":EXIT");
    cl_object result = Cnil;
 
    while (cl_equal(exit_obj, result) == Cnil) {
        printf("\n> ");
        cl_object form = ecl_call("(read)");
        result = cl_safe_eval(form, Cnil, Cnil);
        cl_print(1, result);
    }
    putchar('\n');
 
    return 0;
}

