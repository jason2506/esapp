PY=python

TARGET=esapp.so
SRCS=cython/esapp.pyx cython/segmenter.pxd

all: $(TARGET)

$(TARGET): $(SRCS)
	$(PY) setup.py build_ext --inplace

.PHONY: clean
clean:
	$(RM) -r build
	$(RM) cython/esapp.cpp
	$(RM) $(TARGET)
