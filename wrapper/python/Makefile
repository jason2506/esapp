PY=python

TARGET=esa.so
SRCS=cython/esa.pyx cython/seg.pxd

all: $(TARGET)

$(TARGET): $(SRCS)
	$(PY) setup.py build_ext --inplace

.PHONY: clean
clean:
	$(RM) -r build
	$(RM) cython/esa.cpp
	$(RM) $(TARGET)
