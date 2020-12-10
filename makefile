bin=a.out
obj=main.o ylog_wrapper.o
$(bin):$(obj)
	gcc -g -O0 -o $(bin) $(obj) -lstdc++
main.o:main.c
	gcc -g -O0 -c main.c
ylog_wrapper.o:ylog_wrapper.cc
	gcc -g -O0 -c ylog_wrapper.cc
clean:
	tp a.out *.o
