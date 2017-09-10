#pragma once

class Program
{
public:
	Program();
	~Program();
	Program(const Program& g) = delete;
	Program(const Program&& g) = delete;
	Program& operator= (const Program& g) = delete;
	Program& operator= (const Program&& g) = delete;

	bool link(const std::string& vertexFilename, const std::string& fragmentFilename);
	unsigned int getShader() const { return shader; }
	template<class T>
	void setValue(const std::string& name, T value) const;
	void setVecValue(const std::string& name, float v1, float v2, float v3, float v4) const;

private:
	unsigned int shader;

};
