/******************************************************/
/* Author    : Nourhan Mansour                        */
/* Date      : 18/2/2021                              */
/* Version   : 1.0                                    */
/* File      : compiler.h                             */
/******************************************************/
#ifndef COMPILER_H
#define COMPILER_H

#include "STD_TYPES.h"

/* compiler information */
#define ARMGCC_C_STM32F103C8

/* definition, used for the declaration of local pointers.*/
#define AUTOMATIC

/* This memory class shall be used within type definitions, 
    where no memory qualifier can be specified.
*/
#define TYPEDEF             

/* The compiler abstraction shall provide the NULL_PTR 
    define with a void pointer to zero definition.
*/
#define NULL_PTR          ((void *)0)

/* INLINE define for abstraction of the keyword inline */
#define INLINE            inline

/* the keyword inline in functions with “static” scope. */
#define LOCAL_INLINE      static inline

/*  declaration and definition of functions that ensures correct 
    syntax of function declarations as required by a specific compiler.
*/
#define FUNC(rettype, memclass) rettype



#endif