/*  Auri - A way to set up static Arch Linux machines
    Copyright (C) 2016-2017 Crescent Scroll
    
    This program is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation, either version 3 of the License, or (at your option)
    any later version.
    
    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AURI_TERMINAL_HH
#define AURI_TERMINAL_HH

#include <initializer_list>
#include <set>
#include <string>

namespace auri {

class terminal {
public:
    class node {
    public:
        node(std::initializer_list<std::string> keys);
        
        explicit operator bool() const noexcept;
        bool set() const noexcept;
        
        bool operator()(const std::string& key);
        
    private:
        std::set<std::string> m_keys;
        bool m_set{false};
    };
    
    node help{"-h", "--help"};
    node version{"-v", "--version"};
    
    class: public node {
    public:
        using node::node;
        
        node help{"-h", "--help"};
        
        node nice{"-n", "--nice"};
        node plain{"-p", "--plain"};
    } layout{"layout"};
    
    terminal(const int count, const char* const* vector);
};

}

#endif
