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
    local layout="$(mktemp)"
    
    if ! auri layout --plain > "$layout"; then
        error "Unable to generate device layout"
        exit $?
    fi
    
    add_file "$layout" "/etc/auri/layout"
    rm "$layout"
    
    add_file "/usr/lib/auri/script" "/usr/bin/auri"
    
    add_module "overlay"
    add_runscript
}

help() {
    echo "This hooks allows for booting off temporary filesystems by moving" \
        "specific mount operations to early userspace and creating union" \
        "mounts on top of read-only devices."
}
