#ifndef LOG_H
#define LOG_H

#include<cstdint>
#include<memory>
#include<sstream>
#include<vector>
#include<iostream>
#include<map>
#include<time.h>
#include<fstream>
#include<thread>

#define WARN_LOG(logger,msg)\
    std::stringstream ss; ss << std::this_thread::get_id(); uint32_t id = std::stoull(ss.str()); \
    std::shared_ptr<Log::LogEvent> event(new Log::LogEvent(msg,__FILE__,__LINE__,id,(int64_t)time(NULL),Log::LogLevel::WARN));\
    logger.log(Log::LogEvent::ptr(event));

#define ERROR_LOG(logger,msg)\
    std::stringstream ss; ss << std::this_thread::get_id(); uint32_t id = std::stoull(ss.str()); \
    std::shared_ptr<Log::LogEvent> event(new Log::LogEvent(msg,__FILE__,__LINE__,id,(int64_t)time(NULL),Log::LogLevel::ERROR));\
    logger.log(Log::LogEvent::ptr(event));

#define DEBUG_LOG(logger,msg)\
    std::stringstream ss; ss << std::this_thread::get_id(); uint32_t id = std::stoull(ss.str()); \
    std::shared_ptr<Log::LogEvent> event(new Log::LogEvent(msg,__FILE__,__LINE__,id,(int64_t)time(NULL),Log::LogLevel::DEBUG));\
    logger.log(Log::LogEvent::ptr(event));

#define FATAL_LOG(logger,msg)\
    std::stringstream ss; ss << std::this_thread::get_id(); uint32_t id = std::stoull(ss.str()); \
    std::shared_ptr<Log::LogEvent> event(new Log::LogEvent(msg,__FILE__,__LINE__,id,(int64_t)time(NULL),Log::LogLevel::FATAL));\
    logger.log(Log::LogEvent::ptr(event));

#define INFO_LOG(logger,msg)\
    std::stringstream ss; ss << std::this_thread::get_id(); uint32_t id = std::stoull(ss.str()); \
    std::shared_ptr<Log::LogEvent> event(new Log::LogEvent(msg,__FILE__,__LINE__,id,(int64_t)time(NULL),Log::LogLevel::INFO));\
    logger.log(Log::LogEvent::ptr(event));

namespace Log {

    class Logger;
    class LogLevel
    {
    public:
        LogLevel() {};
        enum Level {
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            UNKNOWN
        };
        static std::string toString(LogLevel::Level lev)
        {
            switch (lev) {
#define XX(name)\
        case LogLevel::Level::name: \
            return #name;\
            break;
                XX(DEBUG);
                XX(INFO);
                XX(WARN);
                XX(ERROR);
                XX(FATAL);
#undef XX
            default:
                return "UNKNOWN";
            }
            return "UNKNOWN";
        }
    };

    class LogEvent
    {
    public:
        typedef std::shared_ptr<LogEvent> ptr;
        LogEvent(
            std::string msg,
            std::string fileName,
            int32_t line,
            uint32_t threadId,
            int64_t time,
            LogLevel::Level level
        ) :m_level(level), m_ss(msg), m_fileName(fileName), m_line(line), m_threadId(threadId), m_time(time) {};
        virtual ~LogEvent() {};

    public:
        LogLevel::Level getLevel() { return m_level; };
        std::string getFileName() { return m_fileName; };
        uint32_t getThreadId() { return m_threadId; };
        uint32_t getFiberId() { return m_fiberId; };
        uint32_t getTime() { return m_time; };
        uint32_t getElapse() { return m_elapse; };
        int32_t getLineNum() { return m_line; };
        std::string getContent() { return m_ss; };
    private:
        LogLevel::Level m_level;
        std::string m_fileName;
        uint32_t m_threadId;
        uint32_t m_fiberId;
        int64_t m_time;
        uint32_t m_elapse;
        int32_t m_line;
        std::string m_ss;
    };

    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> ptr;
        virtual void log(std::string str) = 0;
    };

    class StdLogAppender :public LogAppender
    {
    public:
        void log(std::string str)override //鎺у埗鍙拌緭鍑�
        {
            std::cout << str;
        }
    };

    class FileLogAppender :public LogAppender
    {
    public:
        FileLogAppender(const char* file_path) :m_filePath(file_path) {};
        void log(std::string str)override //鏂囦欢鏃ュ織杈撳嚭
        {
            std::ofstream ofs;
            ofs.open(m_filePath, std::ios::app);
            if (!ofs.is_open())
            {
                std::cout << "File open error!" << std::endl;
            }
            ofs << str;
            std::cout << str << std::endl;
            ofs.close();
        }
    private:
        const char* m_filePath = "";
    };

    /*
    //  浣跨敤閮ㄥ垎log4j鏍囧噯鐨勬棩蹇楁牸寮�

        d	鐢ㄤ簬杈撳嚭鐨勮褰曚簨浠剁殑鏃ユ湡銆備緥濡傦紝 %d{HH:mm:ss,SSS} 鎴� %d{dd MMM yyyy HH:mm:ss,SSS}.
        F	鐢ㄤ簬杈撳嚭琚彂鍑烘棩蹇楄褰曡姹傦紝鍏朵腑鐨勬枃浠跺悕
        L	鐢ㄤ簬杈撳嚭浠庤鍙戝嚭鏃ュ織璁板綍璇锋眰鐨勮鍙�
        m	鐢ㄤ簬杈撳嚭浣跨敤鏃ュ織浜嬩欢鐩稿叧鑱旂殑搴旂敤绋嬪簭鎻愪緵鐨勬秷鎭�
        n	杈撳嚭骞冲彴鐩稿叧鐨勮鍒嗛殧绗︽垨鏂囧瓧
        p	鐢ㄤ簬杈撳嚭鐨勮褰曚簨浠剁殑浼樺厛绾�
        t	鐢ㄤ簬杈撳嚭鐢熸垚鐨勬棩蹇楄褰曚簨浠剁殑绾跨▼鐨勫悕绉�
        %	鏂囧瓧鐧惧垎鍙�%%灏嗘墦鍗帮紖鏍囧織

    */
    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> ptr;
        LogFormatter(std::string pattern) :m_pattern(pattern) { init(); };
        void init();
        LogEvent::ptr m_event;
        class FormatterItem {
        public:
            virtual std::string format() = 0;
            virtual void setEvent(LogEvent::ptr event) = 0;
        };
        std::vector<std::shared_ptr<FormatterItem>> getItems() { return m_items; }
    private:
        std::string m_pattern = "";
        std::vector<std::shared_ptr<FormatterItem>> m_items;
    };

    class DateFormatterItem :public LogFormatter::FormatterItem
    {
    public:
        DateFormatterItem(const std::string& fmt = "") :m_fmt(fmt) {};
        void setEvent(LogEvent::ptr event)override { m_event = event; };
        std::string format()override
        {
            char buf[80];
            int64_t tt = m_event->getTime();
            time(&tt);
            struct tm* timeinfo = localtime(&tt);
            strftime(buf, 80, m_fmt.substr(1, m_fmt.size() - 2).data(), timeinfo);
            return "Date:" + std::string(buf) + "  ";
        };
    private:
        std::string m_fmt;
        LogEvent::ptr m_event;
    };

    class StringFormatterItem :public LogFormatter::FormatterItem
    {
    public:
        StringFormatterItem(const std::string& fmt = "") :m_fmt(fmt) {};
        void setEvent(LogEvent::ptr event)override { m_event = event; };
        std::string format()override
        {
            return " " + m_fmt + " ";
        }
    private:
        std::string m_fmt;
        LogEvent::ptr m_event;
    };

    class FileNameFormatterItem :public LogFormatter::FormatterItem
    {
    public:
        FileNameFormatterItem(const std::string& fmt = "") :m_fmt(fmt) {};
        void setEvent(LogEvent::ptr event)override { m_event = event; };
        std::string format()override
        {
            return "File name:" + m_event->getFileName() + "  ";
        }
    private:
        std::string m_fmt;
        LogEvent::ptr m_event;
    };

    class LineFormatterItem :public LogFormatter::FormatterItem
    {
    public:
        LineFormatterItem(const std::string& fmt = "") :m_fmt(fmt) {};
        void setEvent(LogEvent::ptr event)override { m_event = event; };
        std::string format()override
        {
            return "Line number:" + std::to_string(m_event->getLineNum()) + "  ";
        }
    private:
        std::string m_fmt;
        LogEvent::ptr m_event;
    };

    class MessagesFormatterItem :public LogFormatter::FormatterItem
    {
    public:
        MessagesFormatterItem(const std::string& fmt = "") :m_fmt(fmt) {};
        void setEvent(LogEvent::ptr event)override { m_event = event; };
        std::string format()override
        {
            return "Log information:" + m_event->getContent() + "  ";
        }
    private:
        std::string m_fmt;
        LogEvent::ptr m_event;
    };

    class NewLineFormatterItem :public LogFormatter::FormatterItem
    {
    public:
        NewLineFormatterItem(const std::string& fmt = "") :m_fmt(fmt) {};
        void setEvent(LogEvent::ptr event)override { m_event = event; };
        std::string format()override
        {
            return "\n";
        }
    private:
        std::string m_fmt;
        LogEvent::ptr m_event;
    };

    class LevelFormatterItem :public LogFormatter::FormatterItem
    {
    public:
        LevelFormatterItem(const std::string& fmt = "") :m_fmt(fmt) {};
        void setEvent(LogEvent::ptr event)override { m_event = event; };
        std::string format()override
        {
            return "Level:" + LogLevel::toString(m_event->getLevel()) + "  ";
        }
    private:
        std::string m_fmt;
        LogEvent::ptr m_event;
    };

    class ThreadFormatterItem :public LogFormatter::FormatterItem
    {
    public:
        ThreadFormatterItem(const std::string& fmt = "") :m_fmt(fmt) {};
        void setEvent(LogEvent::ptr event)override { m_event = event; };
        std::string format()override
        {
            return "Thread:" + std::to_string(m_event->getThreadId()) + "  ";
        }
    private:
        std::string m_fmt;
        LogEvent::ptr m_event;
    };

    class Logger :public std::enable_shared_from_this<Logger>
    {
    public:
        enum Pattern {
            STD,
            FILE,
            CUSTOM
        };
        typedef std::shared_ptr<Logger> ptr;
        Logger(std::string name = "root");
        Logger(std::string name, Pattern pattern, const char* file_path);
        void log(LogEvent::ptr envent);

        void setPattern(Pattern pattern) { m_pattern = pattern; };
        void setFormat(LogFormatter::ptr formatter) { m_formatter = formatter; };
        void setAppender(LogAppender::ptr appender) { m_appender = appender; };
        void setFilePath(const char* filePath) { m_filePath = filePath; };
        std::string getName() { return m_name; };
    private:
        Pattern m_pattern = STD;
        std::string m_name = "root";
        LogFormatter::ptr m_formatter;
        LogAppender::ptr m_appender;
        const char* m_filePath = "";
    };

}

#endif // LOG_H
