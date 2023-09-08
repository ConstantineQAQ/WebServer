#include "../include/log.h"

namespace ConstantineQAQ{

    // %m -- 消息体
    // %p -- level
    // %r -- 启动后的时间
    // %c -- 日志名称
    // %t -- 线程id
    // %n -- 回车换行
    // %d -- 时间
    // %f -- 文件名
    // %l -- 行号

class MessageFormatItem : public LogFormatter::FormatItem{
public:
    MessageFormatItem(const std::string& str = ""){}
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem{
public:
    LevelFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << LogLevel::ToString(level);
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem{
public:
    ElapseFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getElapse();
    }
};

class NameFormatItem : public LogFormatter::FormatItem{
public:
    NameFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << logger->getName();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem{
public:
    ThreadIdFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getThreadId();
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem{
public:
    FiberIdFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getFiberId();
    }
};

class DateTimeFormatItem : public LogFormatter::FormatItem{
public:
    DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S") : m_format(format){
    }
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
    }
private:
    std::string m_format;
};

class FilenameFormatItem : public LogFormatter::FormatItem{
public:
    FilenameFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getFile();
    }
};

class LineFormatItem : public LogFormatter::FormatItem{
public:
    LineFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << event->getLine();
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem{
public:
    NewLineFormatItem(const std::string& str = ""){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << std::endl;
    }
};

class StringFormatItem : public LogFormatter::FormatItem{
public:
    StringFormatItem(const std::string& str) : m_string(str){}
    void format(std::ostream& os,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << m_string;
    }
private:
    std::string m_string;
};

class TabFormatItem : public LogFormatter::FormatItem{
public:
    TabFormatItem(const std::string& str = ""){}
    void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override
    {
        os << "\t";
    }
};

const char* LogLevel::ToString(LogLevel::Level level) {
    switch(level) {
#define XX(name) \
    case LogLevel::name: \
        return #name;\
        break; 

    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOW";
    }
    return "UNKNOW";
}

Logger::Logger(const std::string &name):m_name(name),m_level(LogLevel::DEBUG) {
    m_formatter.reset(new LogFormatter("%d [%p] %f %l %m %n"));
}
void Logger::Log(LogLevel::Level level, LogEvent::ptr event)
{
    if(level >= m_level)
    {
        auto self = shared_from_this();
        for(auto& i : m_appenders)
        {
            i->Log(self, level, event);
        }
    }
}
void Logger::Debug(LogEvent::ptr event)
{
    Log(LogLevel::Level::DEBUG, event);
}
void Logger::Info(LogEvent::ptr event)
{
    Log(LogLevel::Level::INFO, event);
}
void Logger::Warn(LogEvent::ptr event)
{    
    Log(LogLevel::Level::WARN, event);
}
void Logger::Error(LogEvent::ptr event)
{
    Log(LogLevel::Level::ERROR, event);
}
void Logger::Fatal(LogEvent::ptr event)
{
    Log(LogLevel::Level::FATAL, event);
}
void Logger::addAppender(LogAppender::ptr appender)
{
    if(!appender->getFormatter())
    {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender)
{
    for(auto it = m_appenders.begin(); it != m_appenders.end(); ++it)
    {
        if(*it == appender)
        {
            m_appenders.erase(it);
            break;
        }
    }
}

FileLogAppender::FileLogAppender(const std::string &filename) : m_filename(filename){}
void FileLogAppender::Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
{
    if(level >= m_level)
    {
        m_filestream << m_formatter->format(logger, level, event);
    }
}

bool FileLogAppender::reopen()
{
    if(m_filestream){
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}

void StdoutLogAppender::Log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event)
{
    if(level >= m_level)
    {
        std::cout << m_formatter->format(logger, level, event);
    }
}
LogFormatter::LogFormatter(const std::string &pattern) : m_pattern(pattern){
    init();
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event){
    std::stringstream ss;
    for(auto& i : m_items)
    {
        i->format(ss, logger, level, event);
    }
    return ss.str();
}

// %xxx %xxx{xxx} %%
// "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
// 解析日志格式
void LogFormatter::init() {
	//str, format, type
	std::vector<std::tuple<std::string, std::string, int> > vec;
	std::string nstr;
	for (size_t i = 0; i < m_pattern.size(); ++i) {
		if (m_pattern[i] != '%') {
			nstr.append(1, m_pattern[i]);
			continue;
		}

		if ((i + 1) < m_pattern.size()) {
			if (m_pattern[i + 1] == '%') {
				nstr.append(1, '%');
				continue;
			}
		}

		size_t n = i + 1;
		int fmt_status = 0;
		size_t fmt_begin = 0;

		std::string str;
		std::string fmt;
		while (n < m_pattern.size()) {
			if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
				&& m_pattern[n] != '}')) {
				str = m_pattern.substr(i + 1, n - i - 1);
				break;
			}
			if (fmt_status == 0) {
				if (m_pattern[n] == '{') {
					str = m_pattern.substr(i + 1, n - i - 1);
					fmt_status = 1; //解析格式
					fmt_begin = n;
					++n;
					continue;
				}
			}
			else if (fmt_status == 1) {
				if (m_pattern[n] == '}') {
					fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
					fmt_status = 0;
					++n;
					break;
				}
			}
			++n;
			if (n == m_pattern.size()) {
				if (str.empty()) {
					str = m_pattern.substr(i + 1);
				}
			}
		}

		if (fmt_status == 0) {
			if (!nstr.empty()) {
				vec.push_back(std::make_tuple(nstr, std::string(), 0));
				nstr.clear();
			}
			vec.push_back(std::make_tuple(str, fmt, 1));
			i = n - 1;
		}
		else if (fmt_status == 1) {
			std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
			m_error = true;
			vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
		}
	}

	if (!nstr.empty()) {
		vec.push_back(std::make_tuple(nstr, "", 0));
	}
    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

		XX(m, MessageFormatItem),           //m:消息
		XX(p, LevelFormatItem),             //p:日志级别
		XX(r, ElapseFormatItem),            //r:累计毫秒数
		XX(c, NameFormatItem),              //c:日志名称
		XX(t, ThreadIdFormatItem),          //t:线程id
		XX(n, NewLineFormatItem),           //n:换行
		XX(d, DateTimeFormatItem),          //d:时间
		XX(f, FilenameFormatItem),          //f:文件名
		XX(l, LineFormatItem),              //l:行号
		XX(T, TabFormatItem),               //T:Tab
		XX(F, FiberIdFormatItem),           //F:协程id
#undef XX
	};

	for (auto& i : vec) {
		if (std::get<2>(i) == 0) {
			m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
		}
		else {
			auto it = s_format_items.find(std::get<0>(i));
			if (it == s_format_items.end()) {
				m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
				m_error = true;
			}
			else {
				m_items.push_back(it->second(std::get<1>(i)));
			}
		}
    }
}
}
ConstantineQAQ::LogEvent::LogEvent(const char *line, int32_t m_line, uint32_t elapse, 
                                    uint32_t threadId, uint32_t fiberId, uint64_t time)
                                    :m_line(m_line), m_elapse(elapse), m_threadId(threadId),
                                    m_fiberId(fiberId), m_time(time), m_ss(line){
}