CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		NetworkProbe.o conn/ConnectionAdapter.o conn/PosixSocketLib.o
OBJS +=		proto/ntp/NTPProtocol.o
OBJS +=		proto/dhcp/DhcpProtocol.o
OBJS +=		util/Utils.o util/Debug.o util/Config.o

LIBS =		-lpthread

TARGET =	NetworkProbe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

