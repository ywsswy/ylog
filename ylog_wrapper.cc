// 这个文件是用c++编译的
#include <string>
#include <fstream>
#include <cassert>
#include <ctime>

class YLog{
 private:
  std::ofstream of_;
  int minlevel_;
 public:
  enum Type {
    ADD = 0,
    OVER
  };
  enum Level {
    DEBUG = 0,
    INFO,
    ERR
  };
  YLog(const int level, const std::string &logfile, const int type = YLog::OVER) : minlevel_(level) {
    assert((this->ERR == level || this->INFO == level || this->DEBUG == level) && "Logfile create failed, please check the level(YLog::ERR or YLog::INFO or YLog::DEBUG.");
    if (type == this->ADD) {
      this->of_.open(logfile.c_str(),std::ios_base::out|std::ios_base::app);
    } else if (type == this->OVER) {
      this->of_.open(logfile.c_str(),std::ios_base::out|std::ios_base::trunc);
    } else {
      assert(0 && "Logfile create failed, please check the type(YLog::OVER or YLog::ADD).");
    }
    assert(this->of_.is_open() && "Logfile create failed, please check the logfile's name and path.");
    return;
  }
  ~YLog(){
    if (this->of_.is_open()) {
      this->of_.close();
    }
    return;
  }
  template<typename T> void W(const std::string &codefile, const int codeline, const int level, const std::string &info, const T &value) {
    assert(this->of_.is_open() && "Logfile write failed.");
    if (this->minlevel_ <= level)
    {
      time_t sectime = time(NULL);
      tm tmtime;
#ifdef _WIN32
#if _MSC_VER<1600
      tmtime = *localtime(&sectime);
#else
      localtime_s(&tmtime, &sectime);
#endif
#else
      localtime_r(&sectime, &tmtime);
#endif
      this->of_ << tmtime.tm_year + 1900 << '-' << tmtime.tm_mon + 1 << '-' << tmtime.tm_mday <<
        ' ' << tmtime.tm_hour << ':' << tmtime.tm_min << ':' << tmtime.tm_sec << " [";
      if (this->ERR == level) {
        this->of_ << "ERROR";
      } else if (this->INFO == level) {
        this->of_ << "INFO";
      } else if (this->DEBUG == level) {
        this->of_ << "DEBUG";
      } else {
        assert(0 && "Log write failed, please check the level(YLog::ERR or YLog::INFO or YLog::DEBUG.");
      }
      this->of_ << "]: [" << codefile << ':' << codeline << "]:" << info << ':' << value << std::endl;
    }
    return;
  }
};
#include "ylog_wrapper.h"
#ifdef __cplusplus
extern "C" {
#endif

class GlobalFunction
{
public:
    static void W(const struct YLogWrapper *log, const char *codefile, const int codeline, const int level, const char *info, const char *value)
    {
        if (log != NULL)
        {
            YLog *ylog = (YLog *)(log->internal_);
            ylog->W(codefile, codeline, level, info, value);
        }
    }
};
struct YLogWrapper *NewYLog(const int level, const char *logfile, const int type)
{
    struct YLogWrapper *log = new struct YLogWrapper;
    log->W = GlobalFunction::W;
    log->internal_ = new YLog(level, logfile, type);
    return log;
}
void DeleteYLog(struct YLogWrapper *log)
{
    if (log != NULL)
    {
        YLog *ylog = (YLog *)(log->internal_);
        delete ylog;
        ylog = NULL;
        delete log;
        log = NULL;
    }
}
#ifdef __cplusplus
};
#endif
