APP_NAME=ShmObjMgr

APP_NAME=ShmObjMgr
APP_CPPFILE = $(wildcard *.cpp)
APP_OBJ += $(APP_CPPFILE:.cpp=.o)
APP_INC= 
APP_LIB= 
APP_FLAGS=-g -Wall 

.PHONY: all clean

all: $(APP_NAME)

%.o: %.cpp
	g++ -c $(APP_FLAGS) $(APP_INC) $< -o $@

$(APP_NAME): $(APP_OBJ)
	g++ -o $(APP_NAME) $(APP_FLAGS)  $^ $(APP_LIB)

clean:
	-$(RM) *.o
	-$(RM) *.gch
	-$(RM) *.d
	-$(RM) $(APP_NAME)

