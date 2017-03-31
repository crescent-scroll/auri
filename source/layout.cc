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

#include <algorithm>
#include <fstream>
#include <iterator>
#include <regex>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace auri {

layout::operator bool() const noexcept
{
    return !empty();
}

bool layout::empty() const noexcept
{
    return m_devices.empty();
}

void layout::verify() const
{
    if (m_devices.empty()) {
        throw std::runtime_error{"undefined device layout"};
    }
    
    if (!m_devices.begin()->parent.empty()) {
        throw std::runtime_error{"bottom entry is not a root device"};
    }
    
    const auto begin=m_devices.begin(), end=m_devices.end();
    
    for (auto iterator=std::next(begin); iterator!=end; ++iterator) {
        if (iterator->parent.empty()) {
            throw std::runtime_error{"multiple root devices"};
        }
        
        const auto match=std::find_if(begin, iterator, [&](const auto& device) {
            return device.label==iterator->parent;
        });
        
        if (match==iterator) {
            throw std::runtime_error{
                "undefined parent device '"+iterator->parent+"'"};
        }
    }
}

auto layout::begin() noexcept
{
    return m_devices.begin();
}

auto layout::begin() const noexcept
{
    return m_devices.begin();
}

auto layout::end() noexcept
{
    return m_devices.end();
}

auto layout::end() const noexcept
{
    return m_devices.end();
}

void layout::insert(entry device)
{
    auto snapshot=m_devices;
    
    try {
        m_devices.erase(device); // override previous entry
        m_devices.insert(std::move(device));
        
        verify();
    } catch (...) {
        m_devices=std::move(snapshot);
        throw;
    }
}

void layout::remove(const entry& device)
{
    auto snapshot=m_devices;
    
    try {
        m_devices.erase(device);
        verify();
    } catch (...) {
        m_devices=std::move(snapshot);
        throw;
    }
}

void layout::clear()
{
    m_devices.clear();
}

layout::layout(std::set<entry> devices) noexcept
    : m_devices{std::move(devices)}
{
}

bool operator<(const layout::entry& left, const layout::entry& right)
{
    if (left==right) { // remove duplicates
        return false;
    }
    
    if (left.parent.empty()) { // favor the root device
        return true;
    } else if (right.parent.empty()) {
        return false;
    }
    
    if (left.label==right.parent) { // favor parent devices
        return true;
    } else if (right.label==left.parent) {
        return false;
    }
    
    return left.label<right.label; // sort alphabetically
}

bool operator==(const layout::entry& left, const layout::entry& right)
{
    return left.label==right.label;
}

nice::nice(std::string file)
    : m_file{std::move(file)}
{
    load();
}

void nice::load(const std::string& file)
{
    std::ifstream stream{file.empty()?m_file:file};
    
    if (!stream) {
        throw std::runtime_error{"layout not accessible"};
    }
    
    stream >> *this;
}

void nice::save(const std::string& file) const
{
    std::ofstream stream{file.empty()?m_file:file, std::ios::trunc};
    
    if (!stream) {
        throw std::runtime_error{"layout not accessible"};
    }
    
    stream << *this;
    
    if (!stream) {
        throw std::runtime_error{"layout not writeable"};
    }
}

namespace {

layout::entry::type cast(const std::string mode)
{
    if (mode=="volatile") {
        return layout::entry::type::transient;
    }
    
    return layout::entry::type::persistent;
}

std::string cast(layout::entry::type mode)
{
    if (mode==layout::entry::type::transient) {
        return "volatile";
    }
    
    return "persistent";
}

std::vector<std::string> match(const std::string& string,
    const std::regex& expression, const std::vector<int>& indices)
{
    std::vector<std::string> matches;
    
    const std::sregex_token_iterator begin{string.begin(),
        string.end(), expression, indices}, end;
    
    std::for_each(begin, end, [&](auto match) {
        matches.push_back(std::move(match));
    });
    
    return matches;
}

void check(const std::string& string, const std::regex& expression)
{
    auto matches=match(string, expression, {-1});
    
    matches.erase(std::remove(matches.begin(), matches.end(), std::string{}),
        matches.end());
    
    if (!matches.empty()) {
        std::string token;
        {
            std::istringstream stream{matches.front()};
            stream >> token;
        }
        
        throw std::runtime_error{"unexpected token '"+token+"'"};
    }
}

template<typename Container>
auto pop(Container& sequence)
{
    auto value=std::move(*sequence.begin());
    sequence.erase(sequence.begin());
    
    return value;
}

template<typename Container>
auto pair(Container sequence)
{
    using type=decltype(pop(sequence));
    
    std::vector<std::pair<type, type>> pairs;
    
    while (sequence.size()>=2) {
        auto key=pop(sequence); // order of parameter evaluation is undefined
        auto value=pop(sequence);
        
        pairs.push_back(std::make_pair(std::move(key), std::move(value)));
    }
    
    if (!sequence.empty()) {
        throw std::runtime_error{"uneven sequence"};
    }
    
    return pairs;
}

layout::entry wrap(std::string mode,
    std::string label,
    std::string parent, std::vector<std::pair<std::string, std::string>> pairs)
{
    layout::entry device{cast(mode), std::move(label), std::move(parent)};
    
    for (auto& pair:pairs) {
        if (pair.first=="identifier") {
            device.identifier=std::move(pair.second);
        } else if (pair.first=="mount-point") {
            device.mount_point=std::move(pair.second);
        } else if (pair.first=="options") {
            device.options=std::move(pair.second);
        } else {
            throw std::runtime_error{"unrecognized key '"+pair.first+"'"};
        }
    }
    
    return device;
}

}

std::istream& operator>>(std::istream& stream, nice& table)
{
    auto matches=[&]() {
        const std::string content{std::istreambuf_iterator<char>{stream},
            std::istreambuf_iterator<char>{}};
        
        if (!stream) {
            throw std::runtime_error{"layout not readable"};
        }
        
        static const std::regex expression{"[\\s]*"
            "(volatile|persistent)" // mode
            "[\\s]+device[\\s]+"
            "([^\\s\\:]+)" // label
            "[\\s\\:]+"
            "([^\\s\\{]*)" // parent
            "[\\s\\{]+"
            "([^\\}]*)\\}" // key/value pairs
            "[\\s]*"};
        
        check(content, expression);
        
        return match(content, expression, {1, 2, 3, 4});
    } ();
    
    std::vector<std::vector<std::pair<std::string, std::string>>> pairs;
    {
        constexpr auto step=4;
        
        for (auto iterator=std::next(matches.begin(), step-1);
            iterator<matches.end(); std::advance(iterator, step-1)) {
            static const std::regex expression{"[\\s]*"
                "([\\S]+)" // key
                "[\\s]+"
                "'([\\S]*)'" // value
                "[\\s]*"};
            
            check(*iterator, expression);
            
            pairs.push_back(pair(match(*iterator, expression, {1, 2})));
            matches.erase(iterator);
        }
    }
    
    while (matches.size()>=3 && !pairs.empty()) {
        auto mode=pop(matches); // order of parameter evaluation is undefined
        auto label=pop(matches);
        auto parent=pop(matches);
        
        table.insert(wrap(std::move(mode),
            std::move(label), std::move(parent), pop(pairs)));
    }
    
    if (!matches.empty() || !pairs.empty()) {
        throw std::runtime_error{"invalid layout"};
    }
    
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const nice& table)
{
    if (!table) {
        return stream;
    }
    
    const auto section=[&](const auto& device) {
        stream << cast(device.mode) << " device " << device.label;
        
        if (!device.parent.empty()) {
            stream << ": " << device.parent;
        }
        
        stream << " {" << std::endl;
        
        const auto line=[](const std::string& key, const auto& value) {
            return "    "+key+" '"+value+'\'';
        };
        
        stream << line("identifier", device.identifier) << std::endl
            << line("mount-point", device.mount_point) << std::endl;
        
        if (!device.options.empty()) {
            stream << line("options", device.options) << std::endl;
        }
        
        stream << '}' << std::endl;
    };
    
    const auto begin=table.begin(), end=table.end();
    
    section(*begin);
    std::for_each(std::next(begin), end, [&](const auto& device) {
        stream << std::endl;
        section(device);
    });
    
    return stream;
}

plain::plain(const layout& table)
    : layout{{table.begin(), table.end()}}
{
}

std::ostream& operator<<(std::ostream& stream, const plain& table)
{
    if (table) {
        stream << "devices='";
        
        const auto begin=table.begin(), last=std::prev(table.end());
        
        std::for_each(begin, last, [&](const auto& device) {
            stream << device.label << ' ';
        });
        
        stream << last->label << '\'' << std::endl;
    }
    
    for (const auto& device:table) {
        stream << std::endl;
        
        const auto line=[&](const auto& key, const auto& value) {
            return device.label+'_'+key+"='"+value+'\'';
        };
        
        stream << line("mode", cast(device.mode)) << std::endl
            << line("identifier", device.identifier) << std::endl
            << line("mount_point", device.mount_point) << std::endl;
        
        if (!device.options.empty()) {
            stream << line("options", device.options) << std::endl;
        }
    }
    
    return stream;
}

}
