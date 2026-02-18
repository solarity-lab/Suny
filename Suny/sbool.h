#ifndef SBOOL_H
#define SBOOL_H

/*
    This file defines the boolean object in Suny.

    A boolean object represents a logical value and can only be
    either true or false (internally represented as 1 or 0).

    Boolean objects are used in conditional expressions such as
    if-statements and control flow operations.
*/

#include "sobj.h"
#include "stype.h"

/*
    Return the global true object.
    Boolean objects are singletons.
*/

SUNY_API struct Sobj* Sobj_make_true();

/*
    Return the global false object.
    Boolean objects are singletons.
*/

SUNY_API struct Sobj* Sobj_make_false();

/*
    creat a true object and false object depends on value
    if value is 0 then return false object
    else return true object
*/

SUNY_API struct Sobj* Sobj_make_bool(int value);

#endif // SBOOL_H