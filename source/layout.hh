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

#ifndef AURI_LAYOUT_HH
#define AURI_LAYOUT_HH

#include <iosfwd>
#include <set>
#include <string>

namespace auri {

class layout {
public:
    struct entry {
        enum class type {
            transient, persistent
        };
        
        type mode;
        std::string label;
        std::string parent;
        std::string identifier;
        std::string mount_point;
        std::string options;
    };
    
    explicit operator bool() const noexcept;
    bool empty() const noexcept;
    
    void verify() const;
    
    auto begin() noexcept;
    auto begin() const noexcept;
    
    auto end() noexcept;
    auto end() const noexcept;
    
    void insert(entry device);
    void remove(const entry& device);
    
    void clear();
    
protected:
    layout(std::set<entry> devices={}) noexcept;
    
private:
    std::set<entry> m_devices;
};

extern bool operator<(const layout::entry& left, const layout::entry& right);
extern bool operator==(const layout::entry& left, const layout::entry& right);

class nice: public layout {
public:
    nice(std::string file);
    
    void load(const std::string& file={});
    void save(const std::string& file={}) const; // requires explicit invocation
    
private:
    const std::string m_file;
};

extern std::istream& operator>>(std::istream& stream, nice& table);
extern std::ostream& operator<<(std::ostream& stream, const nice& table);

class plain: public layout {
public:
    plain(const layout& table);
};

extern std::ostream& operator<<(std::ostream& stream, const plain& table);

}

#endif
