#!/bin/bash

# Auri - A way to set up static Arch Linux machines
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

build() {
    add_dir /mnt/auri/core
    add_dir /mnt/auri/shell
    
    add_runscript
}

help() {
    echo 'This hook mounts a temporary filesystem on top of the read-only' \
        'mounted root device.'
}
