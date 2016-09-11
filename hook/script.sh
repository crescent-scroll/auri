#!/bin/ash

# Auri - A way to set up static Arch Linux machines
# Copyright (C) 2016 Crescent Scroll
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
    poll_device "$(resolve_device $root)" 10
    mount_handler=mount_root
}

fsck_root() {
    echo &> /dev/null
}

mount_root() {
    local device=$(resolve_device "$root") surface="$1"
    
    local core=/mnt/auri/core shell=/mnt/auri/shell
    
    mount -t auto -o ro "$device" "$core"
    mount -t tmpfs auri "$shell"
    
    mkdir "$shell/data"
    mkdir "$shell/work"
    
    mount -t overlay \
        -o "lowerdir=$core,upperdir=$shell/data,workdir=$shell/work" \
        auri "$surface"
}
