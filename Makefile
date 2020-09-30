sources := $(wildcard client/net/*.cpp  client/util/*.cpp client/*.cpp main.cpp)
objects := $(sources:%.cpp=%.o)

executable := ssdb-console
#objects1 := $(notdir $(objects))

#objects2 := $(addprefix ./obj/,$(objects1))

target_dir :=./target/

CC=g++
FLAGS= -Wall

first : before $(executable)
#	echo objects $(objects)
#	echo sources $(sources)
#	echo 'target:' $@


before :
	mkdir -p target
#	echo $(target_dir)executable


ssdb-console : $(objects)
	$(CC) $(FLAGS) -o $@ $^
#	echo $(target_dir)ssdb-console
	rm $(objects)
	mv $(executable) $(target_dir)

.PHONY : clean

clean :
	rm   $(target_dir)$(executable)