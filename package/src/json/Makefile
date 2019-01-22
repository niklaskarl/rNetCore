
JSON_OBJECTS = jtoken.o jvalue.o jstring.o jarray.o jobject.o parser.o decoder.o

all:libjson.a

libjson.a:$(JSON_OBJECTS)
	$(AR) crs $@ $^

.cpp.o:
	$(CXX) -c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $< -o $@

.PHONY: all
