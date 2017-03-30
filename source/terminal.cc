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

#include "terminal.hh"

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

namespace auri {

terminal::node::node(std::initializer_list<std::string> keys)
    : m_keys{keys}
{
}

terminal::node::operator bool() const noexcept
{
    return set();
}

bool terminal::node::set() const noexcept
{
    return m_set;
}

bool terminal::node::operator()(const std::string& key)
{
    return m_set = std::find(m_keys.begin(), m_keys.end(), key)!=m_keys.end();
}

namespace {

template<typename... Nodes>
bool match(const std::string& argument, Nodes&&... objects)
{
    using list=std::vector<std::reference_wrapper<terminal::node>>;
    
    for (auto element:list{std::forward<Nodes>(objects)...}) {
        if (element(argument)) {
            return true;
        }
    }
    
    return false;
}

}

terminal::terminal(const int count, const char* const* vector)
{
    std::vector<std::string> arguments{vector, count+vector};
    
    if (arguments.size()<2) {
        throw std::runtime_error{"missing arguments"};
    }
    
    arguments.erase(arguments.begin());
    
    if (!match(arguments.front(), help, version, layout)) {
        throw std::runtime_error{"invalid argument '"+arguments.front()+"'"};
    }
    
    arguments.erase(arguments.begin());
    
    if (layout) {
        if (arguments.empty()) {
            throw std::runtime_error{"missing arguments"};
        }
        
        if (!match(arguments.front(), layout.help, layout.nice, layout.plain)) {
            throw std::runtime_error{
                "invalid argument '"+arguments.front()+"'"};
        }
        
        arguments.erase(arguments.begin());
    }
    
    if (!arguments.empty()) {
        throw std::runtime_error{"unexpected token '"+arguments.front()+"'"};
    }
}

}
