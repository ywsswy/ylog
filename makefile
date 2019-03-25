exe=main
obj=main.o
cc=g++ -Wall -O0
$(exe):$(obj)
	$(cc) -o $(exe) $(obj) 
main.o:main.cc
	$(cc) -c main.cc
clean:
	rm -rf *.o main main.exe.*
