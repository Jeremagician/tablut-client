/*
 * Useful functions and macros
 */
#ifndef SHARED_H_
#define SHARED_H_

#define UNUSED(x) (void)x;

/* Stringification */
#define xstr(s) str(s)
#define str(s) #s
#define CLAMP(x, min, max) ((x > max) ? max : ((x < min) ? min : x))

#endif /* SHARED_H_ */
