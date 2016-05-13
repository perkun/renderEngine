CC = nvcc

SRCDIR = src
BUILDDIR = build
PROGRAM_NAME = renderEngine
TARGET = bin/$(PROGRAM_NAME)

CPPEXT = cpp
CEXT = c
CUDAEXT = cu

CFLAGS = -g -c
LIB = -lm -lglfw3 -lGL -lGLU -lGLEW -lX11 -lXi -lXxf86vm -lXrandr -lXcursor -lXinerama -lrt -lpthread -ldl -lfreeimage

SOURCES = $(shell find $(SRCDIR) -type f -name *.$(CPPEXT))
CSOURCES = $(shell find $(SRCDIR) -type f -name *.$(CEXT))
CUDASOURCES = $(shell find $(SRCDIR) -type f -name *.$(CUDAEXT))
HEADERS = $(shell find $(SRCDIR) -type f -name *.h)

OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(CPPEXT)=.o))
# COBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(CEXT)=.o))
CUDAOBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(CUDASOURCES:.$(CUDAEXT)=.o))


$(TARGET): $(OBJECTS) $(CUDAOBJECTS)
	@echo " Linking..."
	@echo ">>>>>>>>>>>>>>>> $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)
	cp src/*.vs bin
	cp src/*.fs bin

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(CUDAEXT)
	@mkdir -p $(BUILDDIR)
	@echo "CUDA>>>>>>>>>>>>>>>>  $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(CPPEXT)
	@mkdir -p $(BUILDDIR)
	@echo "CPP>>>>>>>>>>>>>>>>  $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $^

# $(BUILDDIR)/%.o: $(SRCDIR)/%.$(CEXT)
# 	@mkdir -p $(BUILDDIR)
# 	@echo "C>>>>>>>>>>>>>>>>  $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $^


clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

shared_library:
# 	nvcc -arch=sm_20 -Xcompiler '-fPIC' -dc $(CUDASOURCES)
# 	nvcc -arch=sm_20 -Xcompiler '-fPIC' -dlink $(CUDAOBJECTS) -o link.o
# 	g++ -shared -o lib$(PROGRAM_NAME).so $(CUDAOBJECTS) link.o -L/usr/local/cuda/lib64 -lcudart
	$(CC) -arch=sm_20 --ptxas-options=-v --compiler-options '-fPIC' -o bin/lib$(PROGRAM_NAME).so --shared $(CUDASOURCES) $(SOURCES) $(CSOURCES) $(LIB) -lcudart
	sudo cp bin/lib$(PROGRAM_NAME).so /usr/local/lib
	sudo chmod 755 /usr/local/lib/lib$(PROGRAM_NAME).so
	sudo ldconfig
	sudo cp src/*.h /usr/local/include/$(PROGRAM_NAME)


# 	$(CC)  $(CPPFLAGS) --ptxas-options=-v --compiler-options '-fPIC' $(HEADERS) $(SOURCES) $(LIB)
# 	$(CC) -shared -o librenderEngine.so $(OBJECTS) $(LIB)

