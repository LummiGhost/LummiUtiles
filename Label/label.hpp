#pragma once

///// Struct labels
//#define Class
//#define Interface
//#define Enum
//#define Object
//#define Singleton
//
//
///// Function labels
//#define Constructor
//#define Destructor
//#define Method
//#define Static_Method
//#define Pure
//#define Virtual
//#define NoExcept
//
//
///// Parameter labels
//#define __this__
//#define IN
//#define OUT
//#define Move_IN
//#define Move_OUT
//#define Ref
//#define Const
//
//
///// Variable labels
//#define Public
//#define Protected
//#define Private
//
///// Subroutine labels
//#define Try
//#define Catch(Except) Except
//#define Finally
//#define Infinate_Loop for(;;)
//#define Never_Return


#include "clabels.h"

/// Lifetime labels

#define Store_it
#define Store_it_in_Stack
#define Store_it_in_Heap
#define Do_Not_Store_it

/// Memory safety labels

#define Only_Smart_Ptr_Allowed
#define No_Smart_Ptr_Allowed

/// This part is memory safe.
/// No memory leak, No dangling pointer, No dangling reference, No double free.
/// And all functions called in this part are memory safe.
#define Safe

/// This part is not memory safe.
/// May have memory leak, dangling pointer, dangling reference or double free.
/// Or may have type error, null pointer dereference, buffer overflow, stack overflow, etc.
/// make sure you can handle these problems.
/// And any program which calling this part should be not memory safe.
#define Unsafe
