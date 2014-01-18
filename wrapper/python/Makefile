TARGET=esa.so
SRCS=esa.pyx seg.pxd unicode.pxd wstring.pxd

all: $(TARGET)

$(TARGET): $(SRCS)
	python setup.py build_ext --inplace

.PHONY: clean
clean:
	$(RM) -r build
	$(RM) esa.cpp
	$(RM) $(TARGET)
