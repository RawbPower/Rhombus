#include "rbpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

namespace rhombus {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") 
			return GL_VERTEX_SHADER;
		if (type == "pixel" || type == "fragment")
			return GL_FRAGMENT_SHADER;

		Log::Assert(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string & filepath)
	{
		RB_PROFILE_FUNCTION();

		// Read filepath
		std::string source = ReadFile(filepath);

		// Process result
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		// resources/shaders/Texture.glsl
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string & vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		RB_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		RB_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		RB_PROFILE_FUNCTION();

		// Get a program object.
		GLuint program = glCreateProgram();

		Log::Assert(shaderSources.size() <= 2, "We only support 2 shaders!");
		
		// Keep track of shader IDs
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Create an empty shader handle
			GLuint shader = glCreateShader(type);

			// Send the shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.

				Log::Error("%s", infoLog.data());
				Log::Assert(false, "Shader Compilation Failure!");

				break;
			}

			// Shaders are successfully compiled.
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			// Don't leak shaders either.
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			// Use the infoLog as you see fit.
			Log::Error("%s", infoLog.data());
			Log::Assert(false, "Shader Link Failure!");

			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
			

		// Now time to link them together into a program.
		m_RendererID = program;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		RB_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);				// Point to end of file
			result.resize(in.tellg());				// Where is pointer? aka resize string to size of file
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			Log::Error("Could not open file '%s'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		RB_PROFILE_FUNCTION();

		std::unordered_map <GLenum, std::string> shaderSources;

		const char* typeToken = "#type";				// Look for type token
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);		// Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);			// End of shader type declaration line
			Log::Assert(eol != std::string::npos, "Syntax error!");
			size_t begin = pos + typeTokenLength + 1;		// Start of shader type name (after "#type" keyword)
			std::string type = source.substr(begin, eol - begin);
			Log::Assert(ShaderTypeFromString(type), "Invalid shader type specified!");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);		// Start of shader code after shader type declaration line
			Log::Assert(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos);		// Start of next shader type declaration line
			shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Bind() const
	{
		RB_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		RB_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		RB_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		RB_PROFILE_FUNCTION();

		UploadUniformIntArray(name, values, count);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		RB_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const Vec2& value)
	{
		RB_PROFILE_FUNCTION();

		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const Vec3& value)
	{
		RB_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const Vec4& value)
	{
		RB_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const Mat4& value)
	{
		RB_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const Vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const Vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const Vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		// f in this function name means "float"
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const Mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		// f in this function name means "float" and v means "array of" floats
		glUniformMatrix3fv(location, 1, GL_FALSE, matrix.ToPtr());
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const Mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		// f in this function name means "float" and v means "array of" floats
		glUniformMatrix4fv(location, 1, GL_FALSE, matrix.ToPtr());
	}

}