#include "Log.h"
using namespace Log;

Logger::Logger(std::string name) :m_name()
{
    m_formatter = LogFormatter::ptr(new LogFormatter("%F%d{%Y-%m-%d %H:%M:%S}%p%n  %L%t%m%n"));
    m_appender = LogAppender::ptr(new StdLogAppender());
}

Logger::Logger(std::string name, Pattern pattern, const char* file_path)
{
    m_name = name;
    m_pattern = pattern;
    m_filePath = file_path;
    m_formatter = LogFormatter::ptr(new LogFormatter("%F%d{%Y-%m-%d %H:%M:%S}%p%L%t%m%n"));
    switch (pattern)
    {
    case Pattern::STD:
        m_appender = LogAppender::ptr(new StdLogAppender());
        break;
    case Pattern::FILE:
        m_appender = LogAppender::ptr(new FileLogAppender(file_path));
        break;
    default:
        break;
    }
}

void Logger::log(LogEvent::ptr event)
{
    for (auto i : m_formatter->getItems())
    {
        i->setEvent(event);
        m_appender->log(i->format());
    }
}

void LogFormatter::init()
{
    std::string str = "";
    std::vector<std::pair<std::string, std::string>> initMap;  //记录解析后的格式类型以及参数
    int size = m_pattern.size();
    for (int i = 0; i < size; i++)
    {
        if (m_pattern[i] == '%')
        {
            if (!str.empty())
            {
                initMap.push_back(std::make_pair(str, ""));
                str = "";
            }
            if ((i + 2) < size && m_pattern[i + 2] == '{')
            {
                int temp = i + 2;
                while (temp < size)
                {
                    str += m_pattern[temp];
                    if (m_pattern[temp] == '}')\
                    {
                        std::string tempstr = "%";
                        tempstr += m_pattern[i + 1];
                        initMap.push_back(std::make_pair(tempstr, m_pattern.substr(i + 2, temp - i - 1)));
                        str = "";
                        i = temp;
                        break;
                    }
                    temp++;
                }
                if (!str.empty())
                {
                    initMap.push_back(std::make_pair(str, ""));
                    i = temp;
                    str = "";
                    break;
                }
            }
            else if (i + 1 < size)
            {
                std::string tempstr = "%";
                tempstr += m_pattern[i + 1];
                initMap.push_back(std::make_pair(tempstr, ""));
                i++;
                continue;
            }
            else
                initMap.push_back(std::make_pair("%", ""));
        }
        else {
            str += m_pattern[i];
        }
    }
    if (!str.empty())
    {
        initMap.push_back(std::make_pair(str, ""));
        str = "";
    }

    std::vector<std::pair<std::string, std::string>>::iterator it;
    for (it = initMap.begin(); it != initMap.end(); it++) {
        if (it->first == "%%")
        {
            m_items.push_back(std::shared_ptr<FormatterItem>(new StringFormatterItem(it->second)));
        }
        else if (it->first == "%d")
        {
            m_items.push_back(std::shared_ptr<FormatterItem>(new DateFormatterItem(it->second)));
        }
        else if (it->first == "%F")
        {
            m_items.push_back(std::shared_ptr<FormatterItem>(new FileNameFormatterItem(it->second)));
        }
        else if (it->first == "%L")
        {
            m_items.push_back(std::shared_ptr<FormatterItem>(new LineFormatterItem(it->second)));
        }
        else if (it->first == "%m")
        {
            m_items.push_back(std::shared_ptr<FormatterItem>(new MessagesFormatterItem(it->second)));
        }
        else if (it->first == "%n")
        {
            m_items.push_back(std::shared_ptr<FormatterItem>(new NewLineFormatterItem(it->second)));
        }
        else if (it->first == "%p")
        {
            m_items.push_back(std::shared_ptr<FormatterItem>(new LevelFormatterItem(it->second)));
        }
        else if (it->first == "%t")
        {
            m_items.push_back(std::shared_ptr<FormatterItem>(new ThreadFormatterItem(it->second)));
        }
        else
            m_items.push_back(std::shared_ptr<FormatterItem>(new StringFormatterItem(it->first)));
    }
}
