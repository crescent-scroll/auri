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

log="/run/log/auri"
layout="/etc/auri/layout"

detect() { # determine how to run the script
    for argument in "$@"; do
        if [ "$argument" = "--execute" ]; then
            echo "execution"
            return
        fi
    done
    
    echo "logging"
}

record() { # forward and log an invocation
    local path="$0" arguments="$@"
    
    local output; output="$($path --execute "$@")"
    local code="$?"
    
    mkdir -p "$(dirname $log)"
    
    echo "Invocation '$path $arguments' yields exit code '$code'" >> "$log"
    
    echo "$output" | while read -r line; do
        if [ "$line" != "" ]; then
            echo "> $line" >> "$log"
        fi
    done
    
    if [ "$output" != "" ]; then
        echo "$output"
    fi
    
    exit $code
}

if [ "$(detect "$@")" = "logging" ]; then
    record "$@"
fi

. "$layout"

resolve() { # find the root device alias
    echo "$(set -- $devices && echo $1)"
}

query() { # resolve a layout variable
    local object="$1" property="$2"
    
    if [ "$property" != "" ]; then
        local key="${object}_$property"
    else
        local key="$object"
    fi
    
    eval "local value=\$$key"
    
    if [ "$value" = "" ]; then
        echo "error: no value for key '$key' provided"
        exit 4
    fi
    
    echo "$value"
}

setup() { # mount a device
    local device="$1"
    
    local root="/new_root"
    local base="/mnt/auri/$device"
    
    local core="$base/core" shell="$base/shell"
    
    mkdir -p "$core" "$shell"
    
    if [ "$device" = "$(resolve)" ]; then # root has already been mounted
        if [ "$(query $device mode)" = "read-only" ]; then
            mount --move "$root" "$core"
        else
            return
        fi
    else
        if [ "$(query $device mode)" = "read-only" ]; then
            local option="ro" directory="$core"
        else
            local option="rw" directory="$root/$(query $device mount_point)"
        fi
        
        mkdir -p "$directory"
        
        local identifier="$(query $device identifier)"
        
        if ! mount -t "auto" -o "$option" "$identifier" "$directory"; then
            echo "error: mount operation for device '$device' failed"
            exit 5
        fi
        
        if [ "$(query $device mode)" = "read-write" ]; then
            return
        fi
    fi
    
    mount -t "tmpfs" "auri" "$shell"
    
    local matter="$shell/matter" surface="$shell/surface"
    mkdir -p "$matter" "$surface"
    
    local directory="$root/$(query $device mount_point)"
    mkdir -p "$directory"
    
    mount -t "overlay" \
        -o "lowerdir=$core,upperdir=$surface,workdir=$matter" \
        "auri" \
        "$directory"
    
    mkdir -p "$root/$core" "$root/$shell"
    
    mount --move "$core" "$root/$core"
    mount --move "$shell" "$root/$shell"
    
    mount --bind "$root/$surface" "$root/$shell"
}

if [ "$2" = "query" ]; then
    if [ "$3" = "root" ]; then
        query "$(resolve)" "$4"
    elif [ "$3" != "" ]; then
        query "$3" "$4"
    else
        echo "error: no object specified"
        exit 3
    fi
elif [ "$2" = "setup" ]; then
    for device in $(query "devices"); do
        setup "$device"
    done
elif [ "$2" = "" ]; then
    echo "error: no operation specified"
    exit 1
else
    echo "error: command '$2' not recognized"
    exit 2
fi

exit 0
