    #ifndef _LOG_H_
    #define _LOG_H_

    #include <string> 
    #include <stdint.h>
    #include <memory>
    #include <list>
    #include <fstream>
    #include <vector>
    #include <sstream>
    #include <iostream>
    #include <functional>
    #include <time.h>
    #include <map>
    #include <stdarg.h>
    #include "singleton.h"

    #define CONSTANTINEQAQ_LOG_LEVEL(logger, level) \
        if(logger->getLevel() <= level) \
            ConstantineQAQ::LogEventWrap(ConstantineQAQ::LogEvent::ptr \
            (new ConstantineQAQ::LogEvent(logger, level, __FILE__, __LINE__, 0, ConstantineQAQ::GetThreadId() \
            , ConstantineQAQ::GetFiberId(), time(0)))).getSS()

    #define CONSTANTINEQAQ_LOG_DEBUG(logger) CONSTANTINEQAQ_LOG_LEVEL(logger, ConstantineQAQ::LogLevel::DEBUG)
    #define CONSTANTINEQAQ_LOG_INFO(logger) CONSTANTINEQAQ_LOG_LEVEL(logger, ConstantineQAQ::LogLevel::INFO)
    #define CONSTANTINEQAQ_LOG_WARN(logger) CONSTANTINEQAQ_LOG_LEVEL(logger, ConstantineQAQ::LogLevel::WARN)
    #define CONSTANTINEQAQ_LOG_ERROR(logger) CONSTANTINEQAQ_LOG_LEVEL(logger, ConstantineQAQ::LogLevel::ERROR)
    #define CONSTANTINEQAQ_LOG_FATAL(logger) CONSTANTINEQAQ_LOG_LEVEL(logger, ConstantineQAQ::LogLevel::FATAL)

    #define CONSTANTINEQAQ_LOG_FMT_LEVEL(logger, level, fmt, ...) \
        if(logger->getLevel() <= level) \
            ConstantineQAQ::LogEventWrap(ConstantineQAQ::LogEvent::ptr \
            (new ConstantineQAQ::LogEvent(logger, level, __FILE__, __LINE__, 0, ConstantineQAQ::GetThreadId() \
            , ConstantineQAQ::GetFiberId(), time(0)))).getEvent()->format(fmt, __VA_ARGS__)

    #define CONSTANTINEQAQ_LOG_FMT_DEBUG(logger, fmt, ...) CONSTANTINEQAQ_LOG_FMT_LEVEL(logger, ConstantineQAQ::LogLevel::DEBUG, fmt, __VA_ARGS__)
    #define CONSTANTINEQAQ_LOG_FMT_INFO(logger, fmt, ...) CONSTANTINEQAQ_LOG_FMT_LEVEL(logger, ConstantineQAQ::LogLevel::INFO, fmt, __VA_ARGS__)
    #define CONSTANTINEQAQ_LOG_FMT_WARN(logger, fmt, ...) CONSTANTINEQAQ_LOG_FMT_LEVEL(logger, ConstantineQAQ::LogLevel::WARN, fmt, __VA_ARGS__)
    #define CONSTANTINEQAQ_LOG_FMT_ERROR(logger, fmt, ...) CONSTANTINEQAQ_LOG_FMT_LEVEL(logger, ConstantineQAQ::LogLevel::ERROR, fmt, __VA_ARGS__)
    #define CONSTANTINEQAQ_LOG_FMT_FATAL(logger, fmt, ...) CONSTANTINEQAQ_LOG_FMT_LEVEL(logger, ConstantineQAQ::LogLevel::FATAL, fmt, __VA_ARGS__)

    namespace ConstantineQAQ{
    class Logger;

    // 日志级别
    class LogLevel{
    public:
        enum Level{
            UNKNOW = 0,
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };

        static const char* ToString(LogLevel::Level level);
        static LogLevel::Level FromString(const std::string& str);
    };

    // 日志事件
    class LogEvent{
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char* file
                , int32_t line, uint32_t elapse, uint32_t threadId, uint32_t fiberId
                , uint64_t time);


        const char* getFile() const { return m_file; }
        int32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getFiberId() const { return m_fiberId; }
        uint64_t getTime() const { return m_time; }
        const std::string getContent() const { return m_ss.str(); }
        LogLevel::Level getLevel() const { return m_level; }
        std::stringstream& getSS() { return m_ss; }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }

        void format(const char* fmt, ...);
        void format(const char* fmt, va_list al);
    private:
        LogLevel::Level m_level;        // 日志等级
        std::shared_ptr<Logger> m_logger; // 需要写入的日志器
        const char* m_file = nullptr;   // 文件名
        int32_t m_line = 0;             // 行号
        uint32_t m_elapse = 0;          // 程序启动开始到现在的毫秒数
        uint32_t m_threadId = 0;        // 线程id
        uint32_t m_fiberId = 0;         // 协程id
        uint64_t m_time = 0;            // 时间戳
        std::stringstream m_ss;          // 日志内容
    };

    class LogEventWrap{
    public:
        LogEventWrap(LogEvent::ptr e);
        ~LogEventWrap();
        std::stringstream& getSS();
        LogEvent::ptr getEvent() const { return m_event; }
    private:
        LogEvent::ptr m_event;
    };

    // 日志格式器
    class LogFormatter{
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(const std::string& pattern);
        /**
         * @brief 构造函数
         * @param[in] pattern 格式模板
         * @details 
         *  %m 消息
         *  %p 日志级别
         *  %r 累计毫秒数
         *  %c 日志名称
         *  %t 线程id
         *  %n 换行
         *  %d 时间
         *  %f 文件名
         *  %l 行号
         *  %T 制表符
         *  %F 协程id
         *  %N 线程名称
         *
         *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
         */
        std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    public:
        /**
         * 日志内容项格式化
         */
        class FormatItem {
        public:
            typedef std::shared_ptr<FormatItem> ptr;
            virtual ~FormatItem() {}
            /**
             * @brief 格式化日志到流
             * @param[in, out] os 日志输出流
             * @param[in] logger 日志器
             * @param[in] level 日志等级
             * @param[in] event 日志事件
             */
            //纯虚函数
            virtual void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
        };
        void init();
    private:
        std::string m_pattern;
        std::vector<FormatItem::ptr> m_items;
        // 是否有错误
        bool m_error = false;
    };

    // 日志输出地
    class LogAppender{
    friend class Logger;
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual ~LogAppender() {};

        virtual void Log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

        void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
        LogFormatter::ptr getFormatter() const { return m_formatter; }
        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }
    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        LogFormatter::ptr m_formatter;
    };

    // 日志器
    class Logger : public std::enable_shared_from_this<Logger>{
    friend class LoggerManager;
    public:
        typedef std::shared_ptr<Logger> ptr;
        Logger(const std::string& name = "root");
        void Log(LogLevel::Level level, LogEvent::ptr event);
        void Debug(LogEvent::ptr event);
        void Info(LogEvent::ptr event);
        void Warn(LogEvent::ptr event);
        void Error(LogEvent::ptr event);
        void Fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        LogLevel::Level getLevel() const { return m_level; }
        void setLevel(LogLevel::Level val) { m_level = val; }
        const std::string& getName() const { return m_name; }
    private:
        std::string m_name;             // 日志名称
        LogLevel::Level m_level;        // 日志级别
        LogFormatter::ptr m_formatter;  // 日志格式
        std::list<LogAppender::ptr> m_appenders;    // 日志输出地集合
        Logger::ptr m_root;             // 主日志器
    };

    // 输出到控制台的Appender
    class StdoutLogAppender : public LogAppender{
    public:
        typedef std::shared_ptr<StdoutLogAppender> ptr;
        void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
    private:
    };

    // 定义输出到文件的Appender
    class FileLogAppender : public LogAppender{
    public:
        typedef std::shared_ptr<FileLogAppender> ptr;
        FileLogAppender(const std::string& filename);
        void Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;
        // 重新打开文件，文件打开成功返回true
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };

    // 日志器管理类
    class LoggerManager{
    public:
        LoggerManager();
        typedef std::shared_ptr<LoggerManager> ptr;
        Logger::ptr getLogger(const std::string& name);
        void init();
        Logger::ptr getRoot() const { return m_root; }
    private:
        std::map<std::string, Logger::ptr> m_loggers;
        Logger::ptr m_root;
    };

    typedef ConstantineQAQ::Singleton<LoggerManager> LoggerMgr;
    
    }

    #endif