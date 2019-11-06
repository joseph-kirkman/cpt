#ifndef CPT_SYS_HPP
#define CPT_SYS_HPP

#include <string>
#include "cpt/io.hpp"

namespace cpt {
    class System {
    public:
        static std::string execute(const std::string& program);
        static std::string redirect_input(const Path& program, const Path& input);
    };
    
    class Program {
    public:
        static std::shared_ptr<Program> fromFile(const Path& file); 
        explicit Program(const Path& file);
        virtual ~Program()=default;
        virtual void init()=0;
        virtual std::string run(const Path& input)=0;
        friend std::istream& operator>>(std::istream& is, Program& program);
    protected:
        Path file_;
    };

    class CompiledProgram: public virtual Program {
    public:
        using Program::Program;
        void init() override;
        virtual void compile()=0;
        std::string run(const Path& input) override;
    protected:
        Path exe_;
        Path compiler_;
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
        explicit CppProgram(const Path& file);
        void compile() override;
    };

    class JavaProgram: public CompiledProgram, public InterpretedProgram {
    public:
        explicit JavaProgram(const Path& file);
        void init() override;
        void compile() override;
        std::string run(const Path& input) override;
    };

    class PythonProgram: public InterpretedProgram {
    public:
        explicit PythonProgram(const Path& file);
        std::string run(const Path& input) override;
    };

    class GoProgram: public CompiledProgram {
    public:
        explicit GoProgram(const Path& file);
        void compile() override;
    };
}

#endif //CPT_SYS_HPP