import ylog_for_python
import sys

# level(0:debug, 1:info, 2:err), file, type(0:add, 1:over)
test = ylog_for_python.YLog(1, "test.txt", 0)
# W(file, line, level, info, value)
test.W(__file__, sys._getframe().f_lineno, 2, "hello", "world")
