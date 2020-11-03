%{
#include "ylog.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
template<typename T>
class TD;

const int gStartFunction = 1;
const std::string gStartStr = "1";
const int gEndFunction = 2;
const std::string gEndStr = "2";
YLog g_tree_log(YLog::DEBUG, "tree_log.txt", YLog::ADD);

class Info
{
public:
    std::string function_name;//pretty_function
    std::string debug_info;
    int flag_state; // 1 start, 2 end
};

void Print(const std::list<Info> &debug_list)
{
    std::cout << "********************************************************" << std::endl;
    int i = 0;
    for (auto &&it_debug : debug_list)
    {
        ++i;
        std::cout << "[" << i << "]fun:" << it_debug.function_name << std::endl
            << "info:" << it_debug.debug_info << std::endl;
    }
}

void PrintV2(const std::list<Info> &debug_list)
{
    int tab = 2;
    std::stringstream ss;
    for (auto &&it_debug : debug_list)
    {
        ss << it_debug.function_name;
        for (int i = 0; i < tab; ++i)
        {
            ss << '\t';
        }
    }
    g_tree_log.W(__FILE__, __LINE__, YLog::INFO, "", ss.str());
}

void UpdateDebugTree(std::list<Info> &debug_list, const Info &info)
{
    if (info.flag_state == gStartFunction)
    {
        debug_list.push_back(info);
    }
    else
    {
        bool need_pop = true;
        while (!debug_list.empty() && need_pop)
        {
            auto &&back = debug_list.back();
            if (back.function_name == info.function_name)
            {
                need_pop = false;
            }
            debug_list.pop_back();
        }
    }
    PrintV2(debug_list);
    //Print(debug_list);
}

std::list<Info> g_debug_list;
Info g_now_info;

/*
2020-11-2 22:37:18 [INFO]: [main.cc:13]:watch_a
^[0-9]{4}-[0-9]{1,2}-[0-9]{1,2} [0-9]{1,2}:[0-9]{1,2}:[0-9]{1,2} [(INFO)(DEBUG)(ERR)].*	{ std::cout << "[" << __LINE__ << "]:" << yytext << std::endl; }
*/

//.   {std::cout << __LINE__ << "[" << "]:" << yytext << std::endl; }
%}
  
%%
<<EOF>>	{
    //std::cout << "[" << __LINE__ << "]:" << yytext << std::endl;
    UpdateDebugTree(g_debug_list, g_now_info);
    return 0;
}
^[0-9]{4}\-[0-9]{1,2}\-[0-9]{1,2}\ [0-9]{1,2}:[0-9]{1,2}:[0-9]{1,2}\ \[((INFO)|(DEBUG)|(ERROR))\]:.*	{
    //std::cout << "[" << __LINE__ << "]:" << yytext << std::endl;
    UpdateDebugTree(g_debug_list, g_now_info);
    g_now_info.function_name.clear();
    g_now_info.debug_info.clear();
    g_now_info.flag_state = gStartFunction;
    std::string raw_str(yytext);
    int find_time = 5;
    size_t idx = 0;
    for (int i = 0; i < find_time; ++i)
    {
        idx = raw_str.find_first_of(":", idx + 1);
        //std::cerr << "idx:" << idx << std::endl;
        if (idx == std::string::npos)
        {
            std::cerr << "find : error" << std::endl;
            return -1;
        }
    }
    ++idx;
    if (idx > raw_str.size())
    {
        std::cerr << "find : error" << std::endl;
        return -1;
    }
    size_t idx_at = raw_str.find_first_of("@", idx);
    if (idx_at == std::string::npos)
    {
        //std::cerr << "find @ error, default use all" << std::endl;
        g_now_info.function_name = raw_str.substr(idx);
    }
    else
    {
        g_now_info.function_name = raw_str.substr(idx, idx_at - idx);
        std::string state_str = raw_str.substr(idx_at + 1);
        if (state_str == gStartStr)
        {
            g_now_info.flag_state = gStartFunction;
        }
        else if (state_str == gEndStr)
        {
            g_now_info.flag_state = gEndFunction;
        }
        else
        {
            std::cerr << "state_str error" << std::endl;
            return -1;
        }
    }
}
^.*	{
    //std::cout << "[" << __LINE__ << "]:" << yytext << std::endl;
    g_now_info.debug_info.append(std::string(yytext));
    g_now_info.debug_info.append("\n");
}
%%

int main()
{
    g_now_info.flag_state = gStartFunction;
    std::cerr << "yylex:" << yylex() << std::endl;
    return 0;
}
