#include <iostream>
#include <time.h>
#include <thread>
#include "../include/log.h"
#include "../include/util.h"

int main(){
    // 创建一个日志器
    ConstantineQAQ::Logger::ptr logger(new ConstantineQAQ::Logger);
    // 创建一个输出到控制台的Appender
    logger->addAppender(ConstantineQAQ::LogAppender::ptr(new ConstantineQAQ::StdoutLogAppender));
    // 创建一个输出到文件的Appender 
    ConstantineQAQ::FileLogAppender::ptr file_appender(new ConstantineQAQ::FileLogAppender("./log.txt"));
    // 自定义新的LogFormatter
    ConstantineQAQ::LogFormatter::ptr fmt(new ConstantineQAQ::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    
    logger->addAppender(file_appender);

    file_appender->setLevel(ConstantineQAQ::LogLevel::ERROR);

    // ConstantineQAQ::LogEvent::ptr event(new ConstantineQAQ::LogEvent(logger,logger->getLevel(), __FILE__, __LINE__, 0, ConstantineQAQ::GetThreadId(), ConstantineQAQ::GetFiberId(), time(0)));
    // event->getSS() << "Hello ConstantineQAQ";
    // logger->Log(ConstantineQAQ::LogLevel::DEBUG, event);

    CONSTANTINEQAQ_LOG_INFO(logger) << "Hello ConstantineQAQ";
    CONSTANTINEQAQ_LOG_DEBUG(logger) << "Hello ConstantineQAQ";
    CONSTANTINEQAQ_LOG_WARN(logger) << "Hello ConstantineQAQ";
    CONSTANTINEQAQ_LOG_ERROR(logger) << "Hello ConstantineQAQ";
    CONSTANTINEQAQ_LOG_FATAL(logger) << "Hello ConstantineQAQ";

    auto l = ConstantineQAQ::LoggerMgr::GetInstance()->getLogger("xx");
    auto l2 = ConstantineQAQ::LoggerMgr::GetInstance()->getLogger("yy");
    std::cout << l->getName() << std::endl;
    std::cout << l->getLevel() << std::endl;
    std::cout << l2->getName() << std::endl;
    std::cout << l2->getLevel() << std::endl;
    CONSTANTINEQAQ_LOG_INFO(l) << "xsadadadasdxx";
    CONSTANTINEQAQ_LOG_DEBUG(l) << "xsadadadasdxx";
    CONSTANTINEQAQ_LOG_INFO(l2) << "xsadadadasdyy";

    return 0;
}