CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES = Nitroscape/ecosystinterface.cpp Nitroscape/FARM.cpp Nitroscape/harvInfoClass.cpp Nitroscape/MMSClass.cpp Nitroscape/patchinterface.cpp
SOURCES +=  $(wildcard data/*.cpp) 
SOURCES += $(wildcard base/*.cpp)
SOURCES += $(wildcard building/*.cpp)
SOURCES += $(wildcard tools/*.cpp)
SOURCES += $(wildcard economy/*.cpp) 
SOURCES += $(wildcard event/*.cpp)
SOURCES += $(wildcard fields/*.cpp)
SOURCES += $(wildcard fields/crops/*.cpp)
SOURCES += $(wildcard fields/fieldOperation/*.cpp)
SOURCES += $(wildcard fields/staticCrops/*.cpp)
SOURCES += $(wildcard legislation/*.cpp)
SOURCES += $(wildcard livestock/*.cpp)
SOURCES += $(wildcard manager/*.cpp)
SOURCES += $(wildcard products/*.cpp)
SOURCES += $(wildcard soil/*.cpp)
SOURCES += $(wildcard technics/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Nitroscapes

libnitro.a: $(OBJECTS) $(USER_OBJS)
	ar -r "libnitro.a" $(OBJECTS) $(USER_OBJS) $(LIBS)
.cpp.o:
	$(CC) $(CFLAGS) -DUNIX -DNEWSLIM -DMELCAL -DNITROSCAPE $< -o $@

clean:
	rm -rf *.o base/*.o building/*.o tools/*.o economy/*.o event/*.o fields/*.o fields/crops/*.o fields/fieldOperation/*.o fields/staticCrops/*.o fields/staticCrops/* legislation/*.o manager/*.o Nitroscape/*.o products/*.o soil/*.o technics/*.o
