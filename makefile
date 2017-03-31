# Auri - A way to set up volatile Arch Linux machines
# Copyright (C) 2016-2017 Crescent Scroll
# 
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
# 
# You should have received a copy of the GNU General Public License along with
# this program.  If not, see <http://www.gnu.org/licenses/>.

MAKEFLAGS += --no-builtin-rules --no-builtin-variables --no-print-directory

VERSION := 0.2.0-alpha

SOURCES := $(shell find source -name "*.cc")
OBJECTS := $(patsubst source/%.cc,build/%.o,${SOURCES})

auri: binary/auri

binary/auri: ${OBJECTS} | binary
	clang++ -std=c++1y -Wall -g -DAURI_VERSION=\"${VERSION}\" -o $@ $^

build/%.o: source/%.cc | build
	clang++ -std=c++1y -Wall -MMD -c -DAURI_VERSION=\"${VERSION}\" -o $@ $<

binary build:
	mkdir $@

-include $(patsubst %.o,%.d,${OBJECTS})

clean:
	rm -rf binary
	rm -rf build

.PHONY: auri clean
