#include <fstream>
#include <sstream>

#ifdef _WIN32
#else
#include <sys/stat.h>
#endif

#include "cpt/string.hpp"
#include "cpt/io.hpp"

// Path
namespace cpt {

#ifdef _WIN32
    char Path::delim = '\\';
#else
    char Path::delim = '/';
#endif

    bool Path::is_absolute(const std::string& path){
        return !path.empty() && path.front() == Path::delim;
    }

    Path::Path(): abs_(false){}

    Path::Path(const std::string& path){
#ifdef _WIN32
#else
        path_ = String::split(path, delim);
        abs_ = Path::is_absolute(path);
#endif
    }

    bool Path::empty() const {
        return path_.empty();
    }

    bool Path::exists() const {
#ifdef _WIN32
#else
        struct stat sb{};
        return stat(str().c_str(), &sb) == 0;
#endif
    }

    bool Path::is_dir() const {
        struct stat sb{};

        if(stat(str().c_str(), &sb)){
            return false;
        }

        return S_ISDIR(sb.st_mode);
    }

    bool Path::is_file() const {
        struct stat sb{};

        if(stat(str().c_str(), &sb)){
            return false;
        }

        return S_ISREG(sb.st_mode);
    }

    bool Path::is_absolute() const {
        return abs_;
    }

    Path Path::filename() const {
        if(empty()){
            return Path();
        }
        return Path(path_.back());
    }

    std::string Path::str() const {
        std::ostringstream os;

        if(abs_){
#ifdef _WIN32
#else
        os << delim;
#endif
        }

        for(auto token = path_.begin(); token != path_.end(); ++token){
            os << *token;

            if(token + 1 != path_.end()){
                os << delim;
            }
        }

        return os.str();
    }

    Path Path::operator/(const Path& other) const {
        if(other.abs_){
            throw std::runtime_error("cannot concatenate with absolute path");
        }

        Path result(*this);
        result.path_.insert(result.path_.end(), other.path_.begin(), other.path_.end());
        return result;
    }

    std::istream& operator>>(std::istream& is, Path& path){
        path.path_.clear();
        std::string line;
        std::getline(is, line);
        path.path_ = String::split(line, Path::delim);
        path.abs_ = Path::is_absolute(line);
        return is;
    }

    std::ostream& operator<<(std::ostream& os, const Path& path){
        os << path.str();
        return os;
    }

}

// File
namespace cpt {
    std::string File::read(const Path& path) {
        std::fstream fs(path.str());
        return std::string(std::istreambuf_iterator<char>(fs.rdbuf()), std::istreambuf_iterator<char>());
    }
}