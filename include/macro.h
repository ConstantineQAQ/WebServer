#ifndef __MACRO_H__
#define __MACRO_H__

#include <string.h>
#include <assert.h>

#define CONSTANTINEQAQ_ASSERT(x) \
    if(!(x)) { \
        CONSTANTINEQAQ_LOG_ERROR(CONSTANTINEQAQ_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << ConstantineQAQ::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#define CONSTANTINEQAQ_ASSERT2(x, w) \
    if(!(x)) { \
        CONSTANTINEQAQ_LOG_ERROR(CONSTANTINEQAQ_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << ConstantineQAQ::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif