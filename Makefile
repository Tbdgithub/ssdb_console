OBJS = Hello.o main.o

main: $(OBJS)
	g++  $(OBJS) -o ssdb-console  -std=c++11

edit: $(OBJS)
	g++ -o edit $(OBJS) -std=c++11

Hello.o:   tool/Hello.cpp
	g++ -c tool/Hello.cpp -std=c++11

main.o:   main.cpp
	g++ -c main.cpp -std=c++11

clean:
	rm -rf *.o ssdb-console