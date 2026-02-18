#ifndef SCLASS_H
#define SCLASS_H

#include "stype.h"
#include "score.h"
#include "scopy.h"
#include "sframe.h"

#define CLASS_METHOD_NOT_FOUND -1
#define __INIT__ADDRESS 20
#define __ADD__ADDRESS 21
#define __SUB__ADDRESS 22
#define __MUL__ADDRESS 23
#define __DIV__ADDRESS 24
#define __TO_STR__ADDRESS 25

struct Sclass*
Sclass_new
(void);

int 
Sclass_free
(struct Sclass* sclass);

/*
    Store the member into the class like Dog.name = "Tom"
    Return the class
*/
struct Sclass* 
Sclass_store_member
(struct Sclass* sclass, struct Sframe* frame, struct Sobj* value, int address);

/*
    Get the member from the class
    Return the object
*/
struct Sobj* 
Sclass_get_object
(struct Sclass* sclass, int address);

/*
    Store directly the object into the class, with no check of the address
*/
struct Sobj* 
Sclass_push_obj
(struct Sclass* sclass, struct Sobj* obj);

struct Sobj*
Sobj_make_class(struct Sclass* sclass);

struct Sobj*
Sobj_copy_class(struct Sclass* sclass);

/*
    Extends the class
    This function store warper variables just like super class, the value of the warper variables is from the super class
*/
struct Sclass* 
Sclass_extends_class
(struct Sclass* sclass, struct Sclass* super_class);

/*
    Shared the class
    This store all the vaiables to the class, with no warper
*/
struct Sclass*
Sclass_shared_class
(struct Sclass* sclass, struct Sclass* super_class);

/*
    Creat an instance of the class, and call the __init__ method of the class
*/
struct Sframe*
Sclass_call(struct Sframe* frame, struct Sobj* f_obj);

#endif // SCLASS_H
