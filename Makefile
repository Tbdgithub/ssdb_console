sources := $(wildcard client/net/*.cpp  client/util/*.cpp client/*.cpp main.cpp)
objects := $(sources:%.cpp=%.o)
#objects1 := $(notdir $(objects))

#objects2 := $(addprefix ./obj/,$(objects1))

target_dir :=./target/

CC=g++
FLAGS= -Wall

first : before ssdb-console
#	echo objects $(objects)
#	echo sources $(sources)
#	echo 'target:' $@


before :
	mkdir -p target
	echo $(target_dir)ssdb-console


ssdb-console : $(objects)
	$(CC) $(FLAGS) -o $@ $^
#	echo $(target_dir)ssdb-console
	rm $(objects)
	mv ssdb-console $(target_dir)

.PHONY : clean

clean :
	rm $(target_dir)ssdb-console $(objects)