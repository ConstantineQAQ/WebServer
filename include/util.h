#ifndef __ConstantineQAQ_UTIL_H__
#define __ConstantineQAQ_UTIL_H__

#include <sys/syscall.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

namespace ConstantineQAQ{
    // 获取Linux系统下的线程id
    pid_t GetThreadId();
    // 获取当前协程id
    uint32_t GetFiberId();
}

#endif // __ConstantineQAQ_UTIL_H__