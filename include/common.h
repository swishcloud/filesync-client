#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <cstring>
namespace common
{
    template <typename... Arg>
    std::string string_format(const std::string &format, Arg... args);
    const wchar_t *to_wcstr(const char *orig);
    const char *to_cstr(const wchar_t *orig);
    std::string to_string(const wchar_t *orig);
    const std::wstring full_path(const std::wstring path);
    bool file_exist(const wchar_t *filename);
    void run_file(const wchar_t *filename);
    int find_files(std::wstring path, std::vector<std::wstring> &files);
} // namespace common

template <typename... Arg>
std::string common::string_format(const std::string &format, Arg... args)
{
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size <= 0)
    {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}