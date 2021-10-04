
CC=gcc  -g

C_SOURCE=$(wildcard src/*.c)
C_MAIN=$(wildcard main/*.c)

INCLUDE_PATH=-I ./include
LIB_PATH=-L ./lib -lggy

TARGET=bin/project
LIB_NAME=lib/libggy.so

OBJ=$(patsubst %.c,%.o,$(C_SOURCE)) 

%.o:%.c
	$(CC) -g  $^ -o $@ -fPIC -c $(INCLUDE_PATH) 

$(LIB_NAME):$(OBJ)
	$(CC)  -g -fPIC -shared -o $@ $^

bin/project:$(C_MAIN)   
	$(CC)  - g $^ -o $@ $(INCLUDE_PATH) $(LIB_PATH)
	
.PHONY:clean

clean:
	$(RM) $(TARGET)
	$(RM) src/*.o
	$(RM) $(LIB_NAME)
