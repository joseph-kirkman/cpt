#ifndef CPT_IO_HPP
#define CPT_IO_HPP

#include <iostream>
#include <string>
#include <vector>

namespace cpt {

    class Path {
    public:
        Path();
        explicit Path(const std::string& path);
        bool empty() const;
        bool exists() const;
        bool is_dir() const;
        bool is_file() const;
        bool is_absolute() const;
        Path filename(bool with_extension=true) const;
        Path dirname() const;
        Path add_suffix(const std::string& str) const;
        std::string str() const;

        Path operator/(const Path& other) const;

        friend std::istream& operator>>(std::istream& is, Path& path);
        friend std::ostream& operator<<(std::ostream& os, const Path& path);
    public:
        static Path current_path();
    private:
        static bool is_absolute(const std::string& path);
    private:
        static char delim;
    private:
        std::vector<std::string> path_;
        bool                     abs_;
    };

    class File {
    public:
        static std::string read(const Path& path);
        static void write(const Path& path, const std::string& data);
    };
}

#endif //CPT_IO_HPP