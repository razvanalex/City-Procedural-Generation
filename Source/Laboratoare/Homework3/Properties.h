#pragma once
#include <include/glm.h>
#include "Attribute.h"
#include <iostream>

class Properties {
public:
	Shader *shader;
	std::unordered_map<std::string, Atribute*> props;
	std::unordered_map<std::string, Atribute> tempProps;

public: 
	Properties(Shader *shader) {
		this->shader = shader;
		this->props = std::unordered_map<std::string, Atribute*>();
		this->tempProps = std::unordered_map<std::string, Atribute>();
	}

	~Properties() {
		for (auto p : props) {
			delete p.second;
		}
	}

	Properties* Set(std::string name, Atribute *atr) {
		if (props[name] != NULL) {
			delete props[name];
		}

		props[name] = atr;

		return this;
	}

	Properties* Set(std::string name, Atribute atr) {
		tempProps[name] = atr;

		return this;
	}

	void ApplyAll() {
		int location;

		for (auto& p : props) {
			location = glGetUniformLocation(shader->program, p.first.c_str());
			p.second->Apply(location);
		}

		for (auto& p : tempProps) {
			location = glGetUniformLocation(shader->program, p.first.c_str());
			p.second.Apply(location);
		}
	}

	void Finish() {
		tempProps.clear();
	}
};