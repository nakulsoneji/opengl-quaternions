CXX := g++

SRCDIR := src
BINDIR := bin
INCDIR := include
OBJDIR := obj

EXE := $(BINDIR)/app
SRC := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%.o, $(basename $(SRC)))

CXXFLAGS := -I$(INCDIR)
# Note that CPP stands for C Pre Processor, not C++, compilation stage
#CPPFLAGS :=
# use for -L, linking stage
#LDFLAGS :=
#use for -l, linking stage
LDLIBS := -lGL -lglfw

$(EXE): $(OBJ)
	$(CXX) $^ -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

.PHONY: run
run: $(EXE)
	./$(EXE)

.PHONY: clean
clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
