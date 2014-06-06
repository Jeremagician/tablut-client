#ifndef ARRAY2D_H_
#define ARRAY2D_H_

/*
 * Fast way to allocate two dimensional arrays
 * Avoid extra malloc calls and cache misses
 *
 * To free the array, a single call to stdlib free
 * is needed.
 */
#define array2d_alloc(arr, type, width, height)                         \
    do                                                                  \
    {                                                                   \
        arr = malloc(sizeof(*arr) * height + /* Initial padding */      \
                     height * width * sizeof(**arr)); /* Datas  */      \
        if(arr)                                                         \
        {                                                               \
            size_t i;                                                   \
            for(i = 0; i < (size_t)height; i++)                         \
                arr[i] = (type*)(arr + height) + i*width;               \
        }                                                               \
    } while (0)

#endif /* ARRAY2D_H_ */
