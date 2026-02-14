#ifndef SBUILTIN_H
#define SBUILTIN_H

#include "score.h"
#include "stool.h"
#include "slist.h"
#include "sio.h"
#include "scopy.h"
#include "sc_api_func.h"

SUNY_API struct Sframe*
Svm_run_call_context
(struct Scall_context *context);

struct Sc_api_func;

/*
    print function.
    Pops one object from the current frame stack and prints it.
    If the object is a class instance and defines a meta_f_tostring
    method, that method is invoked to obtain the string representation.
    Otherwise, the object is printed directly.
    Always returns null_obj.
*/
SUNY_API struct Sobj* Sprint(struct Sframe* frame);

/*
    exit function.
    Terminates the program.
    Takes no arguments and does not return.
*/
SUNY_API struct Sobj* Sexit(struct Sframe* frame);

/*
    puts function. 1 argument
    Pops one object from the stack and writes it to the output
    without appending a newline.
    Returns null_obj.
*/
SUNY_API struct Sobj* Sputs(struct Sframe* frame);

/*
    read function. 1 argument
    Expects one argument as a prompt, reads a line from
    standard input, strips the trailing newline if present,
    and returns a string object.
*/
SUNY_API struct Sobj* Sread(struct Sframe* frame);

/*
    size function. 1 argument
    Return the size of object
    If the object is a list, return the size of the list
    If the object is a string, return the size of the string
    If none of the above, return 0
*/
SUNY_API struct Sobj* Ssize(struct Sframe* frame);

/*
    push function. 1 argument
    Push the object to the top of the list
    This function return the list itself
*/
SUNY_API struct Sobj* Spush(struct Sframe* frame);

/*
    pop function.
    Take the object from the top of the list, remove it and return it
*/
SUNY_API struct Sobj* Spop(struct Sframe* frame);

/*
    load function. 3 arguments
    First argument is the path to the library file (.dll or .so)
    Second argument is the name of the function
    Third argument is the arguments of the function, using list object to represent the arguments like [arg1, arg2, arg3]
*/
SUNY_API struct Sobj* Sload(struct Sframe *frame);

/*
    isdigit function. 1 argument
    Check if the string object is a digit
*/
SUNY_API struct Sobj* Sisdigit_builtin(struct Sframe *frame);

/*
    range function. 2 arguments
    First argument is the start of the range
    Second argument is the end of the range
    Creat a list object from start to end, just like python
    For example, range(i, j) will return [i, i + 1, i + 2, ..., j - 1, j]
*/
SUNY_API struct Sobj* Srange(struct Sframe *frame);

/*
    copy function. 1 argument
    Creat a copy object from the object and return it
*/
SUNY_API struct Sobj* Scopy(struct Sframe *frame);

/*
    cast function. 1 argument
    Cast the object to list object
*/
SUNY_API struct Sobj* Slist_cast(struct Sframe* frame);

/*
    cast function. 1 argument
    Cast the object to string object
*/
SUNY_API struct Sobj* Sstring_cast(struct Sframe* frame);

/*
    cast function. 1 argument
    Cast the object to char like object, such as "A", "B", "C",...
*/
SUNY_API struct Sobj* Schar_cast(struct Sframe* frame);

/*
    cast function. 1 argument
    Cast the object to number object that has value is an integer
*/
SUNY_API struct Sobj* Sint_cast(struct Sframe* frame);

/*
    cast function. 1 argument
    Cast the object to number object that has value is a float    
*/
SUNY_API struct Sobj* Sfloat_cast(struct Sframe* frame);

/*
    cast function. 1 argument
    Cast the object to boolean object
*/
SUNY_API struct Sobj* Sbool_cast(struct Sframe* frame);

/*
    type function. 1 argument
    Return the type of the object
*/
SUNY_API struct Sobj* Stype(struct Sframe* frame);

/*
    system function. 1 argument
    Run the system command
*/
SUNY_API struct Sobj* Ssystem(struct Sframe* frame);

#endif // SBUILTIN_H
