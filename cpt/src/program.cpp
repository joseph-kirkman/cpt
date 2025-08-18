#include <sstream>
#include <unordered_map>
#include "cpt/config.hpp"
#include "cpt/system.hpp"
#include "cpt/program.hpp"

// Program
namespace cpt {

    static std::unordered_map<std::string, std::unique_ptr<ProgramFabric>>& global_fabrics(){
        static std::unordered_map<std::string, std::unique_ptr<ProgramFabric>> fabrics;
        return fabrics;
    }

#define REGISTER_FABRIC(PREFIX, EXTENSION)                                   \
    class PREFIX##Fabric: public ProgramFabric {                             \
    public:                                                                  \
        std::shared_ptr<Program> create(                                     \
            const Path& file,                                                \
            const Config& config,                                            \
            const Path& bin_dir=Path()) override {                           \
            return std::make_shared<PREFIX##Program>(file, config, bin_dir); \
        }                                                                    \
    };                                                                       \
    __attribute__((constructor))                                             \
    static void init##PREFIX##Fabric() {                                     \
        auto& fabrics = global_fabrics();                                    \
        fabrics[EXTENSION].reset(new PREFIX##Fabric());                      \
    }                                                                        \

    std::shared_ptr<Program> Program::create(const Path& file, const Config& config, const Path& bin_dir) {
        auto extension = file.extension();
        auto& fabrics = global_fabrics();
        auto it = fabrics.find(extension);
        if(it == fabrics.end())
            throw std::runtime_error("unknown file extension " + extension);
        return it->second->create(file, config, bin_dir); 
    }

    Program::Program(const Path& file, const Path& dir):
        file_(file), dir_(dir){}

    CppProgram::CppProgram(const Path& file, const Config& config, const Path& dir):
        Program(file, dir), CompiledProgram(file, dir) {     
        compiler_ = Path(config.get("cpp/compiler"));
        flags_ = config.get("cpp/flags");
    }

    JavaProgram::JavaProgram(const Path& file, const Config& config, const Path& dir):
        Program(file, dir), CompiledProgram(file, dir), InterpretedProgram(file, dir) {
        compiler_ = Path(config.get("java/compiler"));
        interpreter_ = Path(config.get("java/interpreter"));
        flags_ = "-d " + dir_.str();
    }

    PythonProgram::PythonProgram(const Path& file, const Config& config, const Path& dir):
        Program(file, dir), InterpretedProgram(file, dir){
        interpreter_ = Path(config.get("python/interpreter"));
    } 

    GoProgram::GoProgram(const Path& file, const Config& config, const Path& dir):
        Program(file, dir), CompiledProgram(file, dir){
        compiler_ = Path(config.get("go/compiler"));
    }

    std::istream& operator>>(std::istream& is, Program& program){
        is >> program.file_;
        return is;
    }

    void CompiledProgram::init(){
        if(!compiler_.is_file()){
            throw std::runtime_error("cannot find compiler " + compiler_.str());
        }
        this->compile();
    }

    void InterpretedProgram::init(){
        if(!interpreter_.is_file()){
            throw std::runtime_error("cannot find interpreter " + interpreter_.str());
        }
    }

    void JavaProgram::init(){
        CompiledProgram::init();
    }

    void CompiledProgram::compile_command(const std::string& command){
        auto result = System::execute(command);
        
        if(result.second != 0){
            throw std::runtime_error("compilation failed with code " + std::to_string(result.second));
        }
    }

    std::string Program::run_command(const std::string& command){
        auto result = System::execute(command);

        if(result.second != 0){
            throw std::runtime_error("execution failed with code " + std::to_string(result.second));
        }

        return result.first;
    }

    void CppProgram::compile(){

        exe_ = dir_ / file_.filename(false);
        std::stringstream ss;
        ss << compiler_ << " " << flags_ << " " << file_ << " -o " << exe_;

        compile_command(ss.str());
    }

    void JavaProgram::compile(){

        exe_ = file_.filename(false);
        std::stringstream ss;
        ss << compiler_ << " " << flags_ << " " << file_;

        compile_command(ss.str());
    }

    void GoProgram::compile(){

        exe_ = dir_ / file_.filename(false);
        std::stringstream ss;
        ss << compiler_ << " build ";
        ss << "-o " << exe_ << " ";
        ss << file_;

        compile_command(ss.str());
    }

    std::string CompiledProgram::run(const Path& input){
        return run_command(System::redirect_input(exe_, input));
    }

    std::string JavaProgram::run(const Path& input){
        std::stringstream ss;
        ss << interpreter_ << " ";
        ss << "-cp " << dir_ << " ";
        ss << exe_;
        return run_command(System::redirect_input(Path(ss.str()), input));
    }

    std::string PythonProgram::run(const Path& input){
        std::stringstream ss;
        ss << interpreter_ << " " << file_;
        return run_command(System::redirect_input(Path(ss.str()), input));
    }

    REGISTER_FABRIC(Cpp, "cpp")
    REGISTER_FABRIC(Java, "java")
    REGISTER_FABRIC(Python, "py")
    REGISTER_FABRIC(Go, "go")
}