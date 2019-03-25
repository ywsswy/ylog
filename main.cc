/*
# 本日志类特点

* 小巧可爱，全程序（YLog.h）仅60多行，使用方便。
* 不定义宏，全局变量等，减少污染，低碳环保。
* 使用标准库，兼容windows和linux平台。
* 多线程安全。（但低本版编译器如 vc++6.0 会使用localtime等线程不安全的函数）
* 可输出每条日志信息的日志级别、输出时间、所在程序文件名、所在代码行数、日志信息。

# 使用介绍

构造函数为 YLog(const int level, const std::string &logfile, const int type = YLog::OVER);
level：指定日志级别下限，ERROR级别（YLog::ERROR）或INFO级别（YLog::INFO），仅当调用日志输出函数的输出级别达到定义下限级别时才会往日志文件中写日志。（例如，下限定义成YLog::ERROR，那么所有 INFO 级别的信息将不会写入日志文件）
logfile：指定输出日志文件名。（如果含路径，请保证路径文件夹存在）
type：指定每一个日志文件的输出方式，每次程序运行追加（YLog::ADD)或覆盖(YLog::OVER)之前的日志文件。

写日志的函数为 template<typename T> \
	void w(const std::string &codefile, const int codeline, \
		const int level, const std::string &info, const T &value);
codefile：固定使用__FILE__宏表示哪个程序文件输出的日志。
codeline：固定使用__LINE__宏表示文件中哪一行代码输出的日志。
level：本条信息的级别。（仅当达到下限时才会写入文件）
info：string类型的任意信息。
value：想写入日志的任意类型的变量的值。（请确保此类型变量重载了'<<'操作符）

*/
#include <iostream>
#include <string>
#include "YLog.h"
int main(){
	//创建日志对象log1，如果文件存在则追加，日志输出下限级别为INFO级别
	YLog log1(YLog::INFO, "log1.txt", YLog::ADD);
	//创建日志对象log2，如果文件存在则覆盖，日志输出下限级别为ERROR级别
	YLog log2(YLog::ERROR, "log2.txt", YLog::OVER);
	int a = 520;
	double b = 13.14;
	std::string c = "I love U.";

	log1.w(__FILE__, __LINE__, YLog::INFO, "watch_a",a);//INFO级别不低于log1的下限INFO级别，正常写入日志文件
	log1.w(__FILE__, __LINE__, YLog::ERROR, "Watch_b",b);//正常写入
	log1.w(__FILE__, __LINE__, YLog::INFO, "watch_c",c);//正常写入

	log2.w(__FILE__, __LINE__, YLog::INFO, "watch_a",a);//INFO级别低于log2的下限ERROR级别，不写入日志
	log2.w(__FILE__, __LINE__, YLog::ERROR, "Watch_b",b);//正常写入
	log2.w(__FILE__, __LINE__, YLog::INFO, "watch_c",c);//不写入日志
	return 0;
}
