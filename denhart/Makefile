#pdp-8 makefile
#Deborah Denhart

CC=g++
CFLAGS = -g -Wall 
INCLUDES = -I.

SRCS = main.cpp alu.cpp common.cpp controlunit.cpp memory.cpp OpTable.cpp 

OBJS = $(SRCS:.c=.o)

MAIN = pdpSim

.PHONY: depend clean

default:	$(MAIN)
			@echo pdpSim compiled
			
$(MAIN):	$(OBJS)
			$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) 

.c.o:
			$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
			
clean:
			$(RM) *.o *~ $(MAIN)
			
depend:		$(SRCS)
			makedepend $(INCLUDES) $^
			
# DO NOT DELETE THIS LINE --
