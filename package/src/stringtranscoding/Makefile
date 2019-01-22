STRINGTRANSCODING_OBJECTS = stringtranscoding.o utf8toutf16.o utf16toutf8.o

all:libstringtranscoding.a

libstringtranscoding.a:$(STRINGTRANSCODING_OBJECTS)
	$(AR) crs $@ $^

.cpp.o:
	$(CXX) -c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $< -o $@

.PHONY: all build
