CC      = icc
#CFLAGS  = -g -O2 -fopenmp -opt-report5 
CFLAGS  = -g -O3 -fopenmp -opt-report6
LDFLAGS = 
EXE = slbe.out
RUN_PARAMS =

all: $(EXE)

$(EXE): slbe.cpp
	$(CC) $(CFLAGS) -xhost $^   -o $@ $(FLAGS)


mic:slbe.cpp
	$(CC) $(CFLAGS) -mmic $^   -o $(EXE) $(FLAGS)


asm:slbe.cpp 
	$(CC) $(CFLAGS) -xhost $^    $(FLAGS) -S

asm-mic:slbe.cpp
	$(CC) $(CFLAGS) -mmic $^    $(FLAGS) -S

dump:$(EXE)
	objdump -d $@ > slbe.dump.txt


### VTune Analysis 
advanced: $(EXE)
	amplxe-cl -c advanced-hotspots -- ./$< $(RUN_PARAMS)

ge: $(EXE)
	amplxe-cl -c general-exploration -- ./$<  $(RUN_PARAMS)

con: $(EXE)
	amplxe-cl -c concurrency -- ./$<  $(RUN_PARAMS)

uops:$(EXE)
	amplxe-cl -c snb-cycles-uops -- ./$<  $(RUN_PARAMS)

port:$(EXE)
	amplxe-cl -c snb-core-port-saturation -- ./$<  $(RUN_PARAMS)
 
mem:$(EXE)
	amplxe-cl -c snb-memory-access -- ./$<  $(RUN_PARAMS)

 
# Advisor Analysis
survey:$(EXE)
	export OMP_NUM_THREADS=1;advixe-cl -collect survey -interval=10 -mrte-mode=auto -target-duration-type=short -data-limit=100 -resume-after=0 -- ./$< 

trip:$(EXE)
	export OMP_NUM_THREADS=1;advixe-cl -collect tripcounts -interval=10 -mrte-mode=auto -target-duration-type=short -data-limit=100   -- ./$<


# command line execution
serial: $(EXE)
	export OMP_NUM_THREADS=1;./$<  $(RUN_PARAMS)

run: $(EXE)
	./$<  $(RUN_PARAMS)

gui:
	amplxe-gui

clean:
	rm -f $(EXE)

nuke:
	rm -f $(EXE)  *.optrpt *.advi* *.vts *.map *.org *.out *.exe lbout.*
	rm -fR e000

