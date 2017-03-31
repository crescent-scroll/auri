#!/bin/ash

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

run_hook() {
    auri_mount_handler() {
        if [ "$(auri query root mode)" = "volatile" ]; then
            rwopt="ro" default_mount_handler "$1"
        else
            rwopt="rw" default_mount_handler "$1"
        fi
    }
    
    mount_handler="auri_mount_handler"
    
    if [ "$(auri query root mode)" = "volatile" ]; then
        fsck_root() {
            echo &> "/dev/null"
        }
    fi
}

run_latehook() {
    auri setup
}
