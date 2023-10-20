#include "../include/constantineQAQ.h"

ConstantineQAQ::Logger::ptr g_logger = CONSTANTINEQAQ_LOG_ROOT();

void run_in_fiber() {
    CONSTANTINEQAQ_LOG_INFO(g_logger) << "run_in_fiber begin";
    ConstantineQAQ::Fiber::YieldToHold();
    CONSTANTINEQAQ_LOG_INFO(g_logger) << "run_in_fiber end";
    ConstantineQAQ::Fiber::YieldToHold();
}

int main(int argc, char** argv) {
    CONSTANTINEQAQ_LOG_INFO(g_logger) << "main begin -1";
    {
        ConstantineQAQ::Fiber::GetThis();
        CONSTANTINEQAQ_LOG_INFO(g_logger) << "main begin";
        ConstantineQAQ::Fiber::ptr fiber(new ConstantineQAQ::Fiber(run_in_fiber));
        fiber->swapIn();
        CONSTANTINEQAQ_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        CONSTANTINEQAQ_LOG_INFO(g_logger) << "main after end";
        fiber->swapIn();
    }
    return 0;
}