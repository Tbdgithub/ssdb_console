OBJS = Hello.o main.o

main: $(OBJS)
	g++  $(OBJS) -o ssdb-console -lstdc++

edit: $(OBJS)
	g++ -o edit $(OBJS)

Hello.o:   tool/Hello.cpp
	g++ -c tool/Hello.cpp



clean:
	rm -rf *.o ssdb-console