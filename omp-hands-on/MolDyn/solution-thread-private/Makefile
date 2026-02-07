# Makefile for the molecular dynamics code

CC        = gcc
CPP       = g++
OPTFLAGS  = -fopenmp -O


CLINKER   = $(CC)
LIBS      = -lm

CFLAGS    = $(OPTFLAGS)

OBJ=o
EXE=
RM=rm

#
# Object files
#
MDOBJS   = dfill.$(OBJ) \
           main.$(OBJ) \
           forces.$(OBJ) \
	   domove.$(OBJ) \
           dscal.$(OBJ) \
	   fcc.$(OBJ) \
	   mkekin.$(OBJ) \
	   mxwell.$(OBJ) \
	   prnout.$(OBJ) \
           random.$(OBJ) \
	   velavg.$(OBJ) 


#
# Compile
#
all: md$(EXE)


md$(EXE): $(MDOBJS)
	$(CLINKER) $(OPTFLAGS) -o $@ $(MDOBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

#
# Clean out object files and the executable.
#
clean:
	$(RM) *.$(OBJ) md$(EXE)
