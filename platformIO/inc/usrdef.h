#ifndef _USR_DEF_H_
#define _USR_DEF_H_

// Data type definitions
typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef signed char     s8;
typedef signed short    s16;
typedef signed int      s32;

typedef float           f32;
typedef double          f64;

typedef unsigned long   osvar_t;

typedef struct{
  u8  step;
  u16 debounce;
}scan_t;


//=========================log show==========================================//
#define LOG_SHOW_I2C                    enable

#define Arduino_Platform                TRUE

#endif



