#ifndef __CLABELS_H__
#define __CLABELS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Struct labels */
/*
* Class
* @modify: struct
*/
#define Class

/*
* An interface is a contract that defines the signature of the functionality.
* All the methods of an interface are abstract.
* You need to implement the methods of an interface in the class.
* @modify: struct, class
*/
#define Interface
//#define Enum
/*
* Object
* @modify: variable, pointer
*/
#define Object

/*
* This is a singleton class.
* @modify: struct, class
*/
#define Singleton


/* Function labels */

/*
* @modify: function
*/
#define Constructor
/*
* @modify: function
*/
#define Destructor
/*
* @modify: function
*/
#define Method
/*
* @modify: function
*/
#define Static_Method

/*
* A pure function has no side effects and its return value is only determined by its input values.
* @modify: function
*/
#define Pure
/*
* @modify: function
*/
#define Virtual
/*
* @modify: function
*/
#define NoExcept


/* Parameter labels */

/*
* This pointer is the object that the member function is being called on.
* @modify: pointer
*/
#define __this__

/*
* This pointer or parameter is an input parameter.
* If this label modify a pointer, the value of the pointer will not be changed.
* @modify: pointer, parameter
*/
#define IN

/*
* This pointer is an output parameter.
* Give a valid pointer to the function.
* @modify: pointer
*/
#define OUT

/*
* This pointer or parameter is an input parameter.
* And the function will take ownership of the pointer.
* DO NOT use the pointer after calling the function.
* @modify: pointer, parameter
*/
#define Move_IN

/*
* This pointer is an output parameter.
* You must give a null pointer to the function.
* You have the ownership of the pointer.
* You are responsible for deleting the pointer.
* @modify: pointer
*/
#define Move_OUT

/*
* A reference parameter (or pointer) is an input parameter.
* The value may be changed by the function.
* @modify: pointer, parameter
*/
#define Ref

/*
* This function will not change the object.
* @modify: pointer, parameter
*/
#define Const


/* Variable labels */

/*
* Free to access.
* @modify: variable, function pointer
*/
#define Public
/*
* Only accessible by the class and its subclasses.
* @modify: variable, function pointer
*/
#define Protected
/*
* Only accessible by the class.
* @modify: variable, function pointer
*/
#define Private

/* Subroutine labels */
#define Try try_
#define Catch(Except) catch_(#Except)_
#define Finally finally_

/*
* An infinite loop.
*/
#define Infinate_Loop for(;;)

/*
* The function will never return.
* Except for the case of an exception.
* @modify: function
*/
#define Never_Return

#ifdef __cplusplus
}
#endif

#endif // __CLABELS_H__