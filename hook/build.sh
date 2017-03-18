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
    local configuration="$(mktemp)"
    
    if ! auri configuration --plain > "$configuration"; then
        error "Device configuration failed"
        exit $?
    fi
    
    add_file "$configuration" "/etc/auri"
    rm "$configuration"
    
    add_file "/usr/lib/auri/script" "/usr/bin/auri"
    
    local modules
    if ! modules="$(auri configuration --modules)"; then
        error "Module configuration failed"
        exit $?
    fi
    
    for module in "overlay" $modules; do
        add_module "$module"
    done
    
    add_runscript
}

help() {
    echo "This hooks allows for booting off temporary filesystems by moving" \
        "specific mount operations to early userspace and creating union" \
        "mounts on top of read-only devices."
}
