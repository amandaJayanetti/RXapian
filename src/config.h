#include <xapian.h>

/* Xapian::RangeProcesor (and subclasses) was introduced in 1.3.6. Therefore, with Xapian versions before 1.3.6, RXapian uses Xapian::ValueRangeProcessor (and subclasses) for supporting range queries. */
#if XAPIAN_MAJOR_VERSION >= 2 || XAPIAN_MINOR_VERSION > 3 || (XAPIAN_MINOR_VERSION == 3 && XAPIAN_REVISION >= 6)
#define XAPIAN_RANGE_PROCESSOR_SUPPORT 
#endif

#if XAPIAN_MAJOR_VERSION >= 2 || XAPIAN_MINOR_VERSION > 3 || (XAPIAN_MINOR_VERSION == 3 && XAPIAN_REVISION >= 2)
#define XAPIAN_OP_MAX_SUPPORT
#endif

