#
# This file is part of rubik_2^3
# Copyright (C) 2008 Alejandro Lorca <alelorca@yahoo.es>
# 
# rubik_2^3 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# rubik_2^3 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with rubik_2^3.  If not, see <http://www.gnu.org/licenses/>.
#
TARGET=rubik_2^3
# For HEPSPEC06
CXXFLAGS=-O2 -fPIC -pthread --static -Wreturn-type
#CXXFLAGS=-O3 --static
CXX=g++
OBJECTS:=run_rubik.o rubik.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): run_rubik.o rubik.o
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< 

clean:
	rm -f *.o $(TARGET)
