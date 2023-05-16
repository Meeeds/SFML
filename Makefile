LIBS=-lsfml-graphics -lsfml-window -lsfml-system
CXX := g++

all: build

%.o: %.cpp
		$(CXX) -c $< -o $@

%.o: %.hpp
		$(CXX) -c $< -o $@

#build: main.o State.o PlayState.o Object.o Player.o
build: main.o
		@echo "** Building the game"
		$(CXX) -o main main.o $(LIBS)