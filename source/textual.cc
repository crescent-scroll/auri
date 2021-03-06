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

#include "textual.hh"

#include <iostream>

namespace auri {

std::ostream& textual::help(std::ostream& stream)
{
    stream << "Usage: auri [-h|--help]" << std::endl
        << "       auri [-v|--version]" << std::endl
        << "       auri <command> [argument...]" << std::endl
        << std::endl
        << "Options:" << std::endl
        << "  -h, --help\t\tDisplay help message" << std::endl
        << "  -v, --version\t\tShow version information" << std::endl
        << std::endl
        << "Commands:" << std::endl
        << "  layout\t\tManage device layout" << std::endl
        << std::endl
        << "Invoke 'auri <command> -h|--help' for specific information"
        << std::endl;
    
    return stream;
}

std::ostream& textual::version(std::ostream& stream)
{
    stream << "Auri version " << AURI_VERSION << std::endl;
    return stream;
}

std::ostream& textual::layout::help(std::ostream& stream)
{
    stream << "Usage: auri layout [option...]" << std::endl
        << std::endl
        << "Options:" << std::endl
        << "  -h, --help\t\tDisplay help message" << std::endl
        << "  -n, --nice\t\tShow layout in nice format" << std::endl
        << "  -p, --plain\t\tShow layout in plain format" << std::endl;
    
    return stream;
}

}
