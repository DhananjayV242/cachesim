# Makefile: Cache simulator in C++

# --- settings ---

SYSTEMC_HOME = /opt/pkg/systemc-2.3.1
SYSTEMC_INC = $(SYSTEMC_HOME)/include
SYSTEMC_LIB = $(SYSTEMC_HOME)/lib-linux64

CC	= g++
CCOPT	= -g -Wall
#CCOPT  += -I$(SYSTEMC_INC)

LDFLAGS = -L$(SYSTEMC_HOME)/lib-linux64
LDFLAGS += -Xlinker -R -Xlinker $(SYSTEMC_HOME)/lib-linux64
LDFLAGS += -lsystemc

SRCDIR = src
OBJDIR = bin
MAINFILE = $(SRCDIR)/main.cpp

SRC_FILES = $(filter-out $(MAINFILE), $(wildcard $(SRCDIR)/*.cpp))
OBJ_FILES = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC_FILES))
# --- targets ---

sim: $(MAINFILE) $(OBJ_FILES) 
	$(CC) $(CCOPT) $^ -o $@

debug: CCOPT += -DDBG
debug: clean sim

test: sim
	./sim 32 8192 4 0 0 0 0 strace.txt

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CC) $(CCOPT) -c $< -o $@
	
$(OBJ_FILES): |$(OBJDIR)

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR) sim 

	

# EOF
