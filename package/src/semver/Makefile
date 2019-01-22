SEMVER_OBJECTS = semanticversion.o

all:libsemver.a

libsemver.a:$(SEMVER_OBJECTS)
	$(AR) crs $@ $^

.cpp.o:
	$(CXX) -c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $< -o $@

.PHONY: all build
