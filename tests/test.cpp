#include <iostream>
#include <time.h>
#include "../include/log.h"

int main(){
    // 创建一个日志器
    ConstantineQAQ::Logger::ptr logger(new ConstantineQAQ::Logger);
    // 创建一个输出到控制台的Appender
    logger->addAppender(ConstantineQAQ::LogAppender::ptr(new ConstantineQAQ::StdoutLogAppender));

    ConstantineQAQ::LogEvent::ptr event(new ConstantineQAQ::LogEvent(__FILE__, __LINE__, 0, 1, 2, time(0)));

    logger->Log(ConstantineQAQ::LogLevel::DEBUG, event);

    return 0;
}