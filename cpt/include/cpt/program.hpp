#ifndef CPT_PROGRAM_HPP
#define CPT_PROGRAM_HPP

#include "cpt/io.hpp"

namespace cpt {

    class Program {
    public:
        static std::shared_ptr<Program> create(const Path& file, const Config& config, const Path& bin_dir=Path());
        Program(const Path& file, const Path& dir=Path());
        virtual ~Program()=default;
        virtual void init()=0;
        virtual std::string run(const Path& input)=0;
        friend std::istream& operator>>(std::istream& is, Program& program);
    protected:
        std::string run_command(const std::string& command);
    protected:
        Path file_;
        Path dir_;
    };

    class ProgramFabric {
    public: 
        virtual ~ProgramFabric()=default;
        virtual std::shared_ptr<Program> create(const Path& file, const Config& config, const Path& bin_dir=Path())=0;
    };

    class CompiledProgram: public virtual Program {
    public:
        using Program::Program;
        void init() override;
        virtual void compile()=0;
        std::string run(const Path& input) override;
    protected:
        void compile_command(const std::string& command);
    protected:
        Path        exe_;
        Path        compiler_;
        std::string flags_;
    };

    class InterpretedProgram: public virtual Program {
    public:
        using Program::Program;
        void init() override;
    protected:
        Path interpreter_;
    };

    class CppProgram: public CompiledProgram {
    public:
        CppProgram(const Path& file, const Config& config, const Path& dir=Path());
        void compile() override;
    };

    class JavaProgram: public CompiledProgram, public InterpretedProgram {
    public:
        JavaProgram(const Path& file, const Config& config, const Path& dir=Path());
        void init() override;
        void compile() override;
        std::string run(const Path& input) override;
    };

    class PythonProgram: public InterpretedProgram {
    public:
        PythonProgram(const Path& file, const Config& config, const Path& dir=Path());
        std::string run(const Path& input) override;
    };

    class GoProgram: public CompiledProgram {
    public:
        GoProgram(const Path& file, const Config& config, const Path& dir=Path());
        void compile() override;
    };
}

#endif //CPT_PROGRAM_HPP