PY=python

TARGET=esa.so
SRCS=esa.pyx seg.pxd

vpath %.pyx cython
vpath %.pxd cython

all: $(TARGET)

$(TARGET): $(SRCS)
	$(PY) setup.py build_ext --inplace

.PHONY: clean
clean:
	$(RM) -r build
	$(RM) cython/esa.cpp
	$(RM) $(TARGET)
