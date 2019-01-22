ifneq ($(strip $(WIN)),)
PLATFORM_PKG_LIBS = -lShlwapi
else
PLATFORM_PKG_LIBS =
endif

PLATFORM_OBJECTS = path.o path.win.o path.unix.o linking.win.o linking.unix.o

all:libpal.a

libpal.a:$(PLATFORM_OBJECTS)
	$(AR) crs $@ $^

.cpp.o:
	$(CXX) -c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $< -o $@

.PHONY: all build
