#include "ShaderProgram.h"

#include "../../utils/FileReader.h"
#include "exceptions/ShaderException.h"

namespace engine {
    void ShaderProgram::create()
    {
        const std::string vertexSource = utils::readFile(vertexPath);
        const std::string fragmentSource = utils::readFile(fragmentPath);

        if (vertexSource.empty() || fragmentSource.empty())
            throw exceptions::ProgramException("Shader source code is empty", 0);

        const unsigned int vertexShader = createShader(vertexSource.c_str(), GL_VERTEX_SHADER);
        const unsigned int fragmentShader = createShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

        programId = glCreateProgram();

        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);
        glLinkProgram(programId);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        assertProgramLinked();
        setupUniforms();
    }
}
