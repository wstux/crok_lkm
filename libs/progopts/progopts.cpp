/*
 * crok_lkm
 * Copyright (C) 2023  Chistyakov Alexander
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <sstream>

#include "progopts/progopts.h"

namespace wstux {
namespace po {

//////////////////////////////////////////////////////////////////////
// class prog_opts::arg

std::string prog_opts::arg::printable_po() const
{
    return std::string(short_po()) + " [ " + std::string(long_po()) + " ]" + (is_switch ? "" : " arg");
}

bool prog_opts::arg::set_value(const char* p_arg)
{
    if (! (bool)p_value) {
        p_value = std::make_shared<value_t>();
    }
    return cvt(*p_value, p_arg);
}

//////////////////////////////////////////////////////////////////////
// class prog_opts

bool prog_opts::has_key(const std::string& po) const
{
    return (m_tokens.find(po) != m_tokens.cend());
}

bool prog_opts::has_value(const std::string& po) const
{
    tokens_list_t::const_iterator it = m_tokens.find(po);
    if (it == m_tokens.cend()) {
        return false;
    }
    return (bool)it->second->p_value;
}

bool prog_opts::insert_impl(const arg::ptr& a)
{
    if (has_error()) {
        return false;
    }

    if (a->po.size() < 3) {
        m_error_msg = "Invalid program option '" + a->po + "'";
        return false;
    }
    if (a->po[2] != ',') {
        m_error_msg = "Invalid program option separator '" + a->po + "'";
        return false;
    }
    if (! m_tokens.emplace(a->long_po(), a).second) {
        m_error_msg = "Exists program option '" + a->po + "'";
        return false;
    }
    if (! m_tokens.emplace(a->short_po(), a).second) {
        m_error_msg = "Exists program option '" + a->po + "'";
        return false;
    }
    m_supported_args.emplace_back(a);
    return true;
}

bool prog_opts::parse(const int argc, char const* const* argv)
{
    if (has_error()) {
        return false;
    }

    if (argc < 1) {
        m_error_msg = "Invalid argc value: '" + std::to_string(argc) + "'";
        return false;
    }

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg_view(argv[i]);
        tokens_list_t::iterator it = m_tokens.find(arg_view);
        if (it == m_tokens.cend()) {
            m_error_msg = "Unsupported key: '" + std::string(arg_view) + "'";
            return false;
        }
        arg& a = *(it->second);
        if ((i + 1) < argc) {
            if (m_tokens.find(std::string_view(argv[i + 1])) == m_tokens.cend()) {
                ++i;
                if (! a.set_value(argv[i])) {
                    m_error_msg = "Invalid value '" + std::string(argv[i]) + "' for arg: '" + std::string(arg_view) + "'";
                    return false;
                }
            } else if (! a.set_value("true")) {
                m_error_msg = "Invalid value for arg: '" + std::string(arg_view) + "'";
                return false;
            }            
        } else {
            if (! a.set_value("true")) {
                m_error_msg = "Invalid value for arg: '" + std::string(arg_view) + "'";
                return false;
            }
        }
    }
    return true;
}

std::string prog_opts::usage() const
{
    if (m_supported_args.size() == 0) {
        return std::string();
    }

    args_list_t::const_iterator it = std::max_element(m_supported_args.cbegin(), m_supported_args.cend(),
                [] (const arg::ptr& a, const arg::ptr& b) -> bool { return a->printable_po().size() < b->printable_po().size(); });
    const size_t max_po_size = (*it)->printable_po().size();

    std::stringstream ss;
    ss << "Allowed options:" << std::endl;
    constexpr size_t tab_size = 2;
    for (const arg::ptr& p_arg : m_supported_args) {
        const size_t delta_size = max_po_size - p_arg->printable_po().size();
        ss << "  " << p_arg->printable_po() << std::string(delta_size + tab_size, ' ') << p_arg->descr << std::endl;
    }
    return ss.str();
}

} // namespace po
} // namespace wstux

