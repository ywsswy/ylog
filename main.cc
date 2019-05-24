#include <iostream>
#include <string>
#include "ylog.h"
int main(){
  //创建日志对象log1，如果文件存在则追加，日志输出下限级别为INFO级别
  YLog log1(YLog::INFO, "log1.txt", YLog::ADD);
  //创建日志对象log2，如果文件存在则覆盖，日志输出下限级别为ERROR级别
  YLog log2(YLog::ERR, "log2.txt", YLog::OVER);
  int a = 520;
  double b = 13.14;
  std::string c = "I love U.";

  log1.W(__FILE__, __LINE__, YLog::INFO, "watch_a",a);//INFO级别不低于log1的下限INFO级别，正常写入日志文件
  log1.W(__FILE__, __LINE__, YLog::ERR, "see_b",b);//正常写入
  log1.W(__FILE__, __LINE__, YLog::INFO, "log_c",c);//正常写入

  log2.W(__FILE__, __LINE__, YLog::INFO, "A",a);//INFO级别低于log2的下限ERROR级别，不写入日志
  log2.W(__FILE__, __LINE__, YLog::ERR, "B",b);//正常写入
  log2.W(__FILE__, __LINE__, YLog::INFO, "C",c);//不写入日志
  return 0;
}
