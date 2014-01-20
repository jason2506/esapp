PY=python

TARGET=esa.so
SRCS=esa.pyx seg.pxd unicode.pxd wstring.pxd

vpath %.pyx cython
vpath %.pxd cython

all: $(TARGET)

$(TARGET): $(SRCS)
	$(PY) setup.py build_ext --inplace

.PHONY: clean
clean:
	$(RM) -r build
	$(RM) $(TARGET)
