#include "../include/constantineQAQ.h"
#include <assert.h>

ConstantineQAQ::Logger::ptr g_logger = CONSTANTINEQAQ_LOG_NAME("system");

void test_assert(){
    CONSTANTINEQAQ_LOG_INFO(g_logger) << ConstantineQAQ::BacktraceToString(10, 2, "    ");
    CONSTANTINEQAQ_ASSERT(0);
    CONSTANTINEQAQ_ASSERT2(0, "test");
    // assert(1);
}

int main(int argc, char** argv){
    test_assert();
    return 0;
}