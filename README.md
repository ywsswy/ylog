# for c compiler

# 本日志类特点

* 小巧可爱，全程序（ylog.h）仅60多行，使用方便。
* 不定义宏，全局变量等，减少污染，低碳环保。
* 使用标准库，兼容windows和linux平台。
* 多线程安全。（但低本版编译器如 vc++6.0 会使用localtime等线程不安全的函数）
* 可输出每条日志信息的日志级别、输出时间、所在程序文件名、所在代码行数、日志信息。

# 使用介绍
```
  int a = 3;
  YLog log1(YLog::INFO, "log.txt", YLog::ADD);
  log1.W(__FILE__, __LINE__, YLog::INFO, "let me watch a's value",a);
```

## 构造函数
```
YLog(const int level, const std::string &logfile, const int type = YLog::OVER);
```
* level：指定日志级别下限，ERROR级别（YLog::ERR）或INFO级别（YLog::INFO）或DEBUG级别（YLog::DEBUG），仅当调用日志输出函数的输出级别达到定义下限级别时才会往日志文件中写日志。（例如，下限定义成YLog::ERR，那么所有 INFO 级别的信息将不会写入日志文件）
* logfile：指定输出日志文件名。（如果含路径，请保证路径文件夹存在）
* type：指定每一个日志文件的输出方式，每次程序运行追加（YLog::ADD)或覆盖(YLog::OVER)之前的日志文件。

## 写日志的函数
```
template<typename T> \
  void W(const std::string &codefile, const int codeline, const int level, const std::string &info, const T &value);
```
* codefile：固定使用__FILE__宏表示哪个程序文件输出的日志。
* codeline：固定使用__LINE__宏表示文件中哪一行代码输出的日志。
* level：本条信息的级别。（仅当达到下限时才会写入文件）
* info：string类型的任意信息。
* value：想写入日志的任意类型的变量的值。（请确保此类型变量重载了'<<'操作符）

# 大家可以继续考虑对功能进行扩展的部分

## avoid large file.
即轮替，作为服务器日志，服务器不出故障是不重启的，半年一年的日志放到一个文件会导致文件过大。

## 时间上可以精确到毫秒级

```
//windows平台下可以参考如下代码：
SYSTEMTIME yst;
::GetLocalTime(&yst);
fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d.%03d",\
    yst.wYear, yst.wMonth, yst.wDay,\
    yst.wHour, yst.wMinute, yst.wSecond, yst.wMilliseconds);

//兼容windows和linux平台可以在普通获取秒时间的基础上加入如下代码获取毫秒部分：
#include <iostream>
#include <sys/timeb.h>
struct timeb tb;
ftime(&tb);
std::cout << tb.millitm;
```

## 参数个数可变的情况可以参考如下代码

```
#include<iostream>
#include<cstdarg>
#include<cstdio>
#include<string>
#include<cassert>
void fun(const char* format, ...){
    char info[400] = {0};//请确保info可以放下转换的字符
    va_list args;
    va_start(args, format);    
    vsprintf(info,format,args);
    va_end(args); 
    std::string s(info);
    if(sizeof(info) <= s.size()){
        assert("parameter too long");
    }   
    std::cout << s << '\n';
}
int main(){
    fun("%s %d %f\n","789012", 34, 67.89);
    return 0;
}
```

## 判断输出日志的路径文件夹是否存在，不存在则创建文件夹，windows平台代码参考如下：

```
std::string cmdstr = "mkdir \"";
cmdstr = cmdstr.append(path);//path为文件名（绝对路径形式）
cmdstr = cmdstr.substr(0, cmdstr.find_last_of('\\'));
struct _stat fileStat;
//判断路径是否存在
if (!((_stat(cmdstr.substr(7).c_str(), &fileStat) == 0) 
      && 
      (fileStat.st_mode & _S_IFDIR)
    )  
    &&
    (cmdstr.size() != 9)
  ){
  cmdstr.append("\"");
  system(cmdstr.c_str());
}
```

## 几种日志格式
```
2020-11-02 22:09:24 [FATAL]: [path/file.cc:156:function]:INFO
[2020-11-02 22:12:18.240648524] [ERROR] [./schedual.sh:349] unknown
2020-11-2 22:37:18 [INFO]: [main.cc:13]:watch_a
520
```
