CLRHOST_OBJECTS = clrhosting.o clrhostwrapper.o appdomainwrapper.o dotnet/win/clrhost_dotnet_win.o dotnet/unix/clrhost_dotnet_unix.o dotnet/appdomain_dotnet.o

all:libclrhost.a

libclrhost.a:$(CLRHOST_OBJECTS)
	$(AR) crs $@ $^

.cpp.o:
	$(CXX) -c $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $< -o $@

.PHONY: all build
