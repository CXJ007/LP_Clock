#ifndef STD_TYPES_H
#define STD_TYPES_H

typedef unsigned char                  uint8;
typedef unsigned short                 uint16;
typedef unsigned long                  uint32;
typedef float                          float32;
typedef double                         float64;

typedef uint8                          Std_ReturnType;
#define E_OK                           (0U)
#define E_NOT_OK                       (1U)

#define STD_ON                         (1U)
#define STD_OFF                        (0U)

#define STD_HIGHT                      (1U)
#define STD_LOW                        (0U)

#define NULL_PTR                       ((void *)0)

#define MS_TO_TICKS( TimeInMs )        ( ( uint32 ) ( ( ( uint32 ) ( TimeInMs )      \
                                       * ( uint32 ) TX_TIMER_TICKS_PER_SECOND )      \
                                       / ( uint32 ) 1000 ) )    

#endif
