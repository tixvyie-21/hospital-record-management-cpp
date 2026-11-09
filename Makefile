CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET   = hospital

all: $(TARGET)

$(TARGET): main.cpp PatientList.cpp PatientList.h Patient.h
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp PatientList.cpp

clean:
	rm -f $(TARGET) patients.txt
