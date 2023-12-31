#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
    private:
        unsigned int m_RendererID;
        std::string m_Filepath;
        // Caching for uniforms
        std::unordered_map<std::string, int> m_UniformLocationCache;

        int GetUniformLocation(const std::string &name);
        unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
        unsigned int CompileShader(unsigned int type, const std::string &source);
        ShaderProgramSource ParseShader(const std::string &filepath);
    public:
        Shader(const std::string &filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        // Set uniforms
        void SetUniform1i(const std::string &name, int v0);
        void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
};