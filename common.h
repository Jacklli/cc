
#ifndef _COMMON_H_

#define ALIGN(size, align)  ((align == 0) ? size: ((size + align - 1) & (~(align - 1))))
#define UINT_MAX	0xffffffff
#define UCC_WC_T        int

#endif
