/*  Auri - A way to set up volatile Arch Linux machines
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

#include "layout.hh"
#include "terminal.hh"
#include "textual.hh"

#include <exception>
#include <iostream>

int main(const int count, const char* const* vector)
{
    try {
        auri::terminal invocation{count, vector};
        
        if (invocation.help) {
            std::cout << auri::textual::help;
        } else if (invocation.version) {
            std::cout << auri::textual::version;
        } else if (invocation.layout.help) {
            std::cout << auri::textual::layout::help;
        } else if (invocation.layout) {
            auri::nice layout{"/etc/auri/layout"};
            
            if (invocation.layout.nice) {
                std::cout << layout;
            } else {
                std::cout << auri::plain{layout};
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "fatal: unexpected error" << std::endl;
        return 2;
    }
    
    return 0;
}
