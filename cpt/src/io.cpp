#include <algorithm>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#else
#include <unistd.h>
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

    Path::Path(const char* path): Path(std::string(path)){}

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

    Path Path::filename(bool with_extension) const {
        if(empty()){
            return Path();
        }
        std::string filename = path_.back();
        size_t index = filename.find_last_of(".");

        if(index == std::string::npos || with_extension){
            return Path(filename);
        }

        return Path(filename.substr(0, index));
    }

    Path Path::dirname() const {
        Path dir;
        dir.abs_ = abs_;

        if(path_.empty()){
            dir.path_.push_back("..");
        } else {
            dir.path_.assign(path_.begin(), path_.end() - 1);
        }

        return dir;
    }

    std::string Path::extension() const {
        std::string ext;

        if(!path_.empty()){
            auto last = path_.back();
            size_t index = last.find_last_of(".");

            if(index != std::string::npos){
                ext = last.substr(index + 1);
            }
        }

        return ext;
    }

    Path Path::add_suffix(const std::string& str) const {
        std::string path_str = this->str();
        size_t ext_index = path_str.find_last_of(".");

        if(ext_index == std::string::npos){
            path_str += str;
        } else {
            path_str = path_str.substr(0, ext_index) + str + path_str.substr(ext_index);
        }

        return Path(path_str);
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

    Path Path::current_path(){
#ifdef _WIN32
#else
        char path[PATH_MAX];
        if (!getcwd(path, PATH_MAX))
            throw std::runtime_error("cannot get current path because of " + std::string(strerror(errno)));
        return Path(path);
#endif
    }

}

// File
namespace cpt {
    std::string File::read(const Path& path){
        std::ifstream fs(path.str());

        if(!fs.is_open()){
            throw std::runtime_error("cannot open file " + path.str());
        }

        std::string result(std::istreambuf_iterator<char>(fs.rdbuf()), std::istreambuf_iterator<char>());
        fs.close();

        return result;
    }
    void File::write(const Path& path, const std::string& data){
        std::ofstream fs(path.str());

        if(!fs.is_open()){
            throw std::runtime_error("cannot open file " + path.str());
        }

        fs << data;
        fs.close();        
    }
}