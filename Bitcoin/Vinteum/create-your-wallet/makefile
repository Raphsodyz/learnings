CXX = g++
CXXFLAGS = -std=c++11 -g
LDFLAGS = -lstdc++ -lsecp256k1 -lssl -lcrypto -ljsoncpp

SRCS = main.cpp src/services/decoder/walletstatedecoder.cpp src/models/descriptordata.cpp src/cli/popen.cpp src/services/balance/walletstatebalance.cpp src/models/utxo.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = wallet_state

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJS): %.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)