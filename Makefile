OBJS = Hello.o main.o strings.o  bytes.o link.o link_redis.o SSDB_client.o SSDB_impl.o

main: $(OBJS)
	g++  $(OBJS) -o ssdb-console  -std=c++11

edit: $(OBJS)
	g++ -o edit $(OBJS) -std=c++11

Hello.o:   tool/Hello.cpp
	g++ -c tool/Hello.cpp -std=c++11

main.o:   main.cpp
	g++ -c main.cpp -std=c++11

strings.o: client/util/strings.h client/util/strings.cpp
	g++ -c  client/util/strings.cpp  -std=c++11
bytes.o: client/util/bytes.h client/util/bytes.cpp
	g++ -c  client/util/bytes.cpp  -std=c++11

link.o: client/net/link.cpp  client/net/link.h
	g++ -c client/net/link.cpp   -std=c++11

link_redis.o:client/net/link_redis.h client/net/link_redis.cpp
	g++ -c  client/net/link_redis.cpp  -std=c++11

SSDB_client.o:client/SSDB_client.h client/SSDB_client.cpp
	g++ -c client/SSDB_client.cpp -o SSDB_client.o -std=c++11

SSDB_impl.o:client/SSDB_impl.h client/SSDB_impl.cpp
	g++ -c  client/SSDB_impl.cpp  -std=c++11

clean:
	rm -rf *.o ssdb-console