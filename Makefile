CXX = clang++
CXXFLAGS = -O3 -std=c++17 -Wno-deprecated-declarations

# Prefer the active conda environment.
PYTHON_PREFIX := $(shell if [ -n "$$CONDA_PREFIX" ]; then echo "$$CONDA_PREFIX"; else python3 -c "import sys; print(sys.prefix)"; fi)

PYTHON_BIN := $(PYTHON_PREFIX)/bin/python

PYTHON_VERSION := $(shell $(PYTHON_BIN) -c "import sys; print(str(sys.version_info.major) + '.' + str(sys.version_info.minor))")

PYTHON_INCLUDE := $(PYTHON_PREFIX)/include/python$(PYTHON_VERSION)
PYTHON_LIBDIR := $(PYTHON_PREFIX)/lib

NUMPY_INCLUDE := $(shell $(PYTHON_BIN) -c "import numpy; print(numpy.get_include())")

INCLUDES = -Iexternal/matplotlib-cpp -I$(PYTHON_INCLUDE) -I$(NUMPY_INCLUDE)

LDFLAGS = -L$(PYTHON_LIBDIR) -lpython$(PYTHON_VERSION) -Wl,-rpath,$(PYTHON_LIBDIR)

TARGET = plot_example
SRC = main.cpp

all:
	@echo "Using Python prefix:  $(PYTHON_PREFIX)"
	@echo "Using Python binary:  $(PYTHON_BIN)"
	@echo "Using Python version: $(PYTHON_VERSION)"
	@echo "Using Python include: $(PYTHON_INCLUDE)"
	@echo "Using Python libdir:  $(PYTHON_LIBDIR)"
	@echo "Using NumPy include:  $(NUMPY_INCLUDE)"
	@test -f "$(PYTHON_INCLUDE)/Python.h" || \
		(echo "ERROR: Python.h not found at $(PYTHON_INCLUDE)/Python.h"; exit 1)
	@test -f "$(NUMPY_INCLUDE)/numpy/arrayobject.h" || \
		(echo "ERROR: numpy/arrayobject.h not found at $(NUMPY_INCLUDE)/numpy/arrayobject.h"; exit 1)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(INCLUDES) $(LDFLAGS)

clean:
	rm -f $(TARGET)