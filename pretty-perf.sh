#!/bin/bash

esc="$(printf '\033')"

sed 's/std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >/std::string/g;
s/std::map<std::string, std::string, std::less<std::string >, std::allocator<std::pair<std::string const, std::string > > >/std::map<std::string, std::string >/g;
s/std::unordered_map<std::string, std::string, std::hash<std::string >, std::equal_to<std::string >, std::allocator<std::pair<std::string const, std::string > > >/std::unordered_map<std::string, std::string >/g
s/std::unordered_map<std::basic_string_view<char, std::char_traits<char> >, std::basic_string_view<char, std::char_traits<char> >, std::hash<std::basic_string_view<char, std::char_traits<char> > >, std::equal_to<std::basic_string_view<char, std::char_traits<char> > >, std::allocator<std::pair<std::basic_string_view<char, std::char_traits<char> > const, std::basic_string_view<char, std::char_traits<char> > > > >/std::unordered_map<std::string_view, std::string_view >/g;
s/std::vector<std::string, std::allocator<std::string > >/std::vector<std::string>/g
s/std::vector<std::basic_string_view<char, std::char_traits<char> >, std::allocator<std::basic_string_view<char, std::char_traits<char> > > >/std::vector<std::string_view>/g' \
    "$1" \
    | sed "s/\([a-zA-Z_0-9]\+\)::/${esc}[35m\1${esc}[0m::/g" \
    | sed "s/::\([a-zA-Z_0-9]\+\)\((\|[^:]\+(inlined\|\[\|$\)/::${esc}[33m\1${esc}[0m\2/g" \
    | sed "s/::\([a-zA-Z_0-9]\+\)\([<,*> ]\)/::${esc}[36m\1${esc}[0m\2/g" \
    | sed "s/\(true\|false\|const\)/${esc}[32m&${esc}[0m/g" \
    | sed "s/\(char\|bool\|string_view\|string\)/${esc}[36m\1${esc}[0m/g" \
    | sed "s/^#.*/${esc}[37m&${esc}[0m/" \
    | less -R -S

