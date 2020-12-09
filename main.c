// 这个文件是用c编译的
#include "ylog_wrapper.h"
#include <stdio.h>

int main(void)
{
    struct YLogWrapper *ylog1 = NewYLog(1, "test.txt", 0);
    ylog1->W(ylog1, __FILE__, __LINE__, 0, "test1", "value1");
    ylog1->W(ylog1, __FILE__, __LINE__, 1, "test1", "value2");
    ylog1->W(ylog1, __FILE__, __LINE__, 2, "test1", "value3");
    return 0;
}
