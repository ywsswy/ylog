bin=a.out
obj=main.o ylog_wrapper.o
$(bin):$(obj)
	gcc -o $(bin) $(obj) -lstdc++
main.o:main.c
	gcc -c main.c
ylog_wrapper.o:ylog_wrapper.cc
	gcc -c ylog_wrapper.cc
clean:
	tp a.out *.o
