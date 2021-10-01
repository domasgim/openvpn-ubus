DEPS = $(wildcard *.h)
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
TARGET = openvpn-ubus
LDFLAGS+=-lblobmsg_json -lubox -lubus -luci
RM = rm -f

all: $(TARGET)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) $(OBJ) $(TARGET)
