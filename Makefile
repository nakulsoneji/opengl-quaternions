CC := gcc
CXX := g++

SRCDIR := src
BINDIR := bin
INCDIR := include
OBJDIR := obj

EXE := $(BINDIR)/app
SRC := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%.o, $(basename $(SRC)))

CFLAGS := -I$(INCDIR) 
CXXFLAGS := -I$(INCDIR)
# Note that CPP stands for C Pre Processor, not C++, compilation stage
#CPPFLAGS :=
# use for -L, linking stage
#LDFLAGS :=
#use for -l, linking stage
LDLIBS := -lGL -lglfw

$(EXE): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CXX) $^ -o $@ $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CFLAGS) 

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) 

.PHONY: run
run: $(EXE)
	./$(EXE)

.PHONY: clean
clean:
	rm -rf $(BINDIR)/*
	rm -rf $(OBJDIR)/*
