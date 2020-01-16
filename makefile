CC = gcc
PLOT = python3

DYN_FLAGS = -lm
DYN_OBJ = source/dynClust.o
DYN_EXEC = source/dynClust

PLOT_OBJ = source/plot.py
PLOT_FLAGS = 

dyn: cleanSource $(DYN_OBJ)
	$(CC) -o $(DYN_EXEC) $(DYN_OBJ) $(DYN_FLAGS)

runDyn: dyn
	clear && ./$(DYN_EXEC)


plot: $(PLOT_OBJ)
	$(PLOT) $(PLOT_FLAGS) $(PLOT_OBJ) $(FILE) $(NODES) $(ARGS)


cleanSource: 
	rm -f source/*.o && rm -f $(DYN_EXEC) && rm -f $(PDIFF_EXEC)
