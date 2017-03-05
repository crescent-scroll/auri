#!/bin/ash

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

run_hook() {
    auri_mount_handler() {
        rwopt=ro default_mount_handler "$1"
        
        fsck_root() {
            echo &> /dev/null
        }
    }
    
    mount_handler=auri_mount_handler
}

run_latehook() {
    local root=/new_root
    
    local core=/mnt/auri/core shell=/mnt/auri/shell
    
    mount --move "$root" "$core"
    mount -t tmpfs tmpfs "$shell"
    
    local data="$shell/data" work="$shell/work"
    
    mkdir -p "$data"
    mkdir -p "$work"
    
    mount -t overlay \
        -o "lowerdir=$core,upperdir=$data,workdir=$work" \
        auri "$root"
    
    mkdir -p "$root/$core"
    mkdir -p "$root/$shell"
    
    mount --bind "$core" "$root/$core"
    mount --bind "$data" "$root/$shell"
    
    local log="$root/var/log"
    
    mkdir -p "$log"
    
    mount > "$log/auri.log"
}
