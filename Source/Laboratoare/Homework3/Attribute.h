#pragma once
#include <include/glm.h>

class Atribute {
public:
	virtual void Apply(int location) {};
};

class Mat4Attribute : public Atribute {
public:
	glm::mat4 value;

	Mat4Attribute(glm::mat4 value) {
		this->value = value;
	}

	~Mat4Attribute() {}

	void Apply(int location) override {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
};

class Mat3Attribute : public Atribute {
public:
	glm::mat3 value;

	Mat3Attribute(glm::mat3 value) {
		this->value = value;
	}

	~Mat3Attribute() {}

	void Apply(int location) override {
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
};

class Vec4Attribute : public Atribute {
public:
	glm::vec4 value;

	Vec4Attribute(glm::vec4 value) {
		this->value = value;
	}

	~Vec4Attribute() {}

	void Apply(int location) override {
		glUniform4fv(location, 1, glm::value_ptr(value));
	}
};

class Vec3Attribute : public Atribute {
public:
	glm::vec3 value;

	Vec3Attribute(glm::vec3 value) {
		this->value = value;
	}

	~Vec3Attribute() {}

	void Apply(int location) override {
		glUniform3fv(location, 1, glm::value_ptr(value));
	}
};

class Vec2Attribute : public Atribute {
public:
	glm::vec2 value;

	Vec2Attribute(glm::vec2 value) {
		this->value = value;
	}

	~Vec2Attribute() {}

	void Apply(int location) override {
		glUniform2fv(location, 1, glm::value_ptr(value));
	}
};

class FloatAttribute : public Atribute {
public:
	float value;

	FloatAttribute(float value) {
		this->value = value;
	}

	~FloatAttribute() {}

	void Apply(int location) override {
		glUniform1f(location, value);
	}
};

class IntAttribute : public Atribute {
public:
	int value;

	IntAttribute(int value) {
		this->value = value;
	}

	~IntAttribute() {}

	void Apply(int location) override {
		glUniform1i(location, value);
	}
};

class TextureAttribute : public Atribute {
public:
	Texture2D* value;
	
	TextureAttribute(Texture2D* value) {
		this->value = value;
	}

	~TextureAttribute() {}

	void Apply(int location) override {
		// Activate texture location 0
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, value->GetTextureID());

		// Send texture uniform value
		glUniform1i(location, 0);
	}
};

class FloatArrayAttribute : public Atribute {
public:
	GLfloat *value;
	int size;

	FloatArrayAttribute(GLfloat *value, int size) {
		this->value = value;
		this->size = size;
	}

	~FloatArrayAttribute() {}

	void Apply(int location) override {
		glUniform1fv(location, size, value);
	}
};

class Vec3ArrayAttribute : public Atribute {
public:
	glm::vec3 *value;
	int size;

	Vec3ArrayAttribute(glm::vec3 *value, int size) {
		this->value = value;
		this->size = size;
	}

	~Vec3ArrayAttribute() {}

	void Apply(int location) override {
		glUniform3fv(location, size, glm::value_ptr(value[0]));
	}
};