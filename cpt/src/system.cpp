#include <array>
#include <sstream>
#include <unordered_map>
#include "cpt/string.hpp"
#include "cpt/system.hpp"


// System
namespace cpt {

    std::string System::execute(const std::string& program) {
        std::array<char, 128> buffer{};
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(program.c_str(), "r"), pclose);

        if (!pipe) {
            throw std::runtime_error("popen() failed");
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return result;
    }

    std::string System::redirect_input(const Path& program, const Path& input){
        std::stringstream ss;

        if(!program.is_absolute()){
            ss << "./";
        }

        ss << program.str() << " <" << input.str();
        return ss.str();
    }

}

// Program
namespace cpt {

    std::shared_ptr<Program> Program::fromFile(const Path& file){
        auto extension = file.extension();

        if(extension == "cpp"){
            return std::make_shared<CppProgram>(file);
        } else if(extension == "py"){
            return std::make_shared<PythonProgram>(file);
        } else if(extension == "java"){
            return std::make_shared<JavaProgram>(file);
        } else if(extension == "go"){
            return std::make_shared<GoProgram>(file);
        } else {
            throw std::runtime_error("unknown file extension " + extension);
        }
    }

    Program::Program(const Path& file):
        file_(file){}

    CppProgram::CppProgram(const Path& file):
        Program(file), CompiledProgram(file){         
        compiler_ = Path("/usr/bin/g++");
    }

    JavaProgram::JavaProgram(const Path& file):
        Program(file), CompiledProgram(file), InterpretedProgram(file){
        compiler_ = Path("/usr/bin/javac");
        interpreter_ = Path("/usr/bin/java");
    }

    PythonProgram::PythonProgram(const Path& file):
        Program(file), InterpretedProgram(file){
        interpreter_ = Path("/usr/bin/python");
    } 

    GoProgram::GoProgram(const Path& file):
        Program(file), CompiledProgram(file){
        compiler_ = Path("/usr/local/go/bin/go");
    }

    std::istream& operator>>(std::istream& is, Program& program){
        is >> program.file_;
        return is;
    }

    void CompiledProgram::init(){
        this->compile();
    }

    void InterpretedProgram::init(){}

    void JavaProgram::init(){
        CompiledProgram::init();
    }

    void CppProgram::compile(){
        exe_ = file_.dirname() / file_.filename(false);
        std::stringstream ss;
        ss << compiler_ << " " << file_ << " -o " << exe_;
        System::execute(ss.str());
    }

    void JavaProgram::compile(){
        exe_ = file_.filename(false);
        std::stringstream ss;
        ss << compiler_ << " " << file_;
        System::execute(ss.str());
    }

    void GoProgram::compile(){
        auto dir = file_.dirname();
        exe_ = dir / file_.filename(false);
        std::stringstream ss;
        ss << compiler_ << " build ";

        if(!dir.empty()){
            ss << "-o " << dir << " ";
        }

        ss << file_;
        System::execute(ss.str());
    }

    std::string CompiledProgram::run(const Path& input){
        return System::execute(System::redirect_input(exe_, input));
    }

    std::string JavaProgram::run(const Path& input){
        auto dir = file_.dirname();
        std::stringstream ss;
        ss << interpreter_ << " ";

        if(!dir.empty()){
            ss << "-cp " << dir << " ";
        }

        ss << exe_;
        return System::execute(System::redirect_input(Path(ss.str()), input));
    }

    std::string PythonProgram::run(const Path& input){
        std::stringstream ss;
        ss << interpreter_ << " " << file_;
        return System::execute(System::redirect_input(Path(ss.str()), input));
    }
}