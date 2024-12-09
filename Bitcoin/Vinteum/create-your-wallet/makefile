CC= gcc
CXX = g++
CFLAGS = -std=c11 -g
CXXFLAGS = -std=c++11 -g
LDFLAGS = -lstdc++ -lsecp256k1 -lssl -lcrypto -ljsoncpp -ldl

SQLITE_SRC = src/libs/sqlite/sqlite3.c
SQLITE_OBJ = $(SQLITE_SRC:.c=.o)

SRCS = main.cpp \
	src/services/decoder/walletstatedecoder.cpp \
	src/models/descriptordata.cpp \
	src/cli/popen.cpp \
	src/services/balance/walletstatebalance.cpp \
	src/models/utxo.cpp \
	src/repository/walletrepository.cpp

OBJS = $(SRCS:.cpp=.o) $(SQLITE_OBJ)
TARGET = wallet_state

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)