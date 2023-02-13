/* try-but-fail.h -- an "always explode" implementation of Mark Adler's try.h.
 */
/* To use, include try-but-fail.h in all locations try.h was used. Essentially,
   exceptions thrown with `throw()` will immediately crash the program.
   Including this file, however, avoids the need to refactor code using try.h
   when one does not expect or particularly want to handle error cases (e.g.,
   benchmarking). */

#ifdef _TRY_H
#error "try-but-fail.h should never be included with try.h"
#endif

#ifndef _TRY_BUT_FAIL_H
#define _TRY_BUT_FAIL_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The exposed names can be changed here. */
#define ball_t try_ball_t_
#define try TRY_TRY_
#define preserve TRY_PRESERVE_
#define always TRY_ALWAYS_
#define catch TRY_CATCH_
#define throw TRY_THROW_
#define punt TRY_PUNT_
#define drop TRY_DROP_

/* Package of an integer code and any other data to be thrown and caught. Here,
   why is a string with information to be displayed to indicate why an
   exception was thrown.  free is true if why was allocated and should be freed
   when no longer needed.  This structure can be customized as needed, but it
   must start with an int code.  If it is customized, the try_throw_() function
   in try.c must also be updated accordingly.  As an example, why could be a
   structure with information for use in the catch block. */
typedef struct {
  int ret;   /* longjmp() return value */
  int code;  /* integer code (required) */
  int free;  /* if true, the message string was allocated */
  char* why; /* informational string or NULL */
} try_ball_t_;

/* Try a block.  The block should follow the invocation of try enclosed in { }.
   The block must be immediately followed by a preserve, always, or catch.  You
   must not goto or return out of the try block.  A naked break or continue in
   the try block will go to the end of the block. */
#define TRY_TRY_ do {
/* Preserve local automatic variables that were changed in the try block by
   reissuing the setjmp(), replacing the state for the next longjmp().  The
   preserve block should be enclosed in { }.  The block must be immediately
   followed by a preserve, always, or catch.  You must not goto or return out
   of the preserve block.  A naked break or continue in the preserve block will
   go to the end of the block.  This can only follow a try or another preserve.
   preserve effectively saves the state of local automatic variables at threat,
   i.e. the register state, at that point so that a subsequent throw() will
   restore those variables to that state for the always and catch blocks.
   Changes to those variables after the preserve statement may or may not be
   reflected in the always and catch blocks. */
#define TRY_PRESERVE_ \
  }                   \
  while (0)           \
    ;                 \
  if (1)              \
    do {
/* Execute the code between always and catch, whether or not something was
   thrown.  An always block is optional.  If present, the always block must
   follow a try or preserve block and be followed by a catch block.  The always
   block should be enclosed in { }.  A naked break or continue in the always
   block will go to the end of the block.  It is permitted to use throw in the
   always block, which will fall up to the next enclosing try.  However this
   will result in a memory leak if the original throw() allocated space for the
   informational string.  So it's best to not throw() in an always block.  Keep
   the always block simple.

   Great care must be taken if the always block uses an automatic storage
   variable local to the enclosing function that can be modified in the try
   block.  Such variables must be declared volatile.  If such a variable is not
   declared volatile, and if the compiler elects to keep that variable in a
   register, then the throw will restore that variable to its state at the
   beginning of the try block, wiping out any change that occurred in the try
   block.  This can cause very confusing bugs until you remember that you
   didn't follow this rule. */
#define TRY_ALWAYS_ \
  }                 \
  while (0)         \
    ;               \
  if (1)            \
    do {
/* Catch an error thrown in the preceding try block.  The catch block must
   follow catch and its parameter, and must be enclosed in { }.  The catch must
   immediately follow a try, preserve, or always block.  It is permitted to use
   throw() in the catch block, which will fall up to the next enclosing try.
   However the ball_t passed by throw() must be freed using drop() before doing
   another throw, to avoid a potential memory leak. The parameter of catch must
   be a ball_t declared in the function or block containing the catch.  It is
   set to the parameters of the throw() that jumped to the catch.  The catch
   block is not executed if the first parameter of the throw() was zero.

   A catch block should end with either a punt() or a drop().

   Great care must be taken if the catch block uses an automatic storage
   variable local to the enclosing function that can be modified in the try
   block.  Such variables must be declared volatile or preserve must be used to
   save their state.  If such a variable is not declared volatile, and if the
   compiler elects to keep that variable in a register, then the throw will
   restore that variable to its state at the beginning of the most recent try
   or preserve block, wiping out any change that occurred after the start of
   that block.  This can cause very confusing bugs until you remember that you
   didn't follow this rule. */
#define TRY_CATCH_(try_ball_) \
  }                           \
  while (0)                   \
    ;                         \
  if (0)

/* Throw by immediately printing the error string and exiting. */
#if defined(__GNUC__) || defined(__has_builtin)
__attribute__((noreturn))
#endif
void try_throw_(int code, char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, fmt, args);
  exit(code);
}

#define TRY_THROW_(...) try_throw_(__VA_ARGS__, NULL)

/* Disallow `punt`. */
#define TRY_PUNT_(try_ball_)                                     \
  do {                                                           \
    fprintf(stderr, "try-but-fail.h: punt should never happen"); \
    exit(1);                                                     \
  } while (0)

/* Disallow `drop`. */
#define TRY_DROP_(try_ball_)                                     \
  do {                                                           \
    fprintf(stderr, "try-but-fail.h: drop should never happen"); \
    exit(1);                                                     \
  } while (0)

#endif /* _TRY_H */
