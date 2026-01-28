#ifndef BITMASK_H
#define BITMASK_H

typedef int bitmask;
#define FLAG(n) (1 << (n))
#define add_flag(b, n) ((b) |= (n))
#define remove_flag(b, n) ((b) &= ~(n))
#define check_flag(b, n) ((b) & (n))

#endif // BITMASK_H