#include "pch.h"
#include "openglobjectsmanager.h"

namespace prev { namespace opengl {

	std::list<Vao> OpenGLObjectsManager::m_Vaos;
	std::list<Vbo> OpenGLObjectsManager::m_Vbos;
	std::list<Ibo> OpenGLObjectsManager::m_Ibos;

	const Vao * OpenGLObjectsManager::CreateVAO() {
		m_Vaos.push_back(Vao());
		return &m_Vaos.back();
	}

	const Vbo* OpenGLObjectsManager::CreateVBO() {
		m_Vbos.push_back(Vbo());
		return &m_Vbos.back();
	}

	const Ibo* OpenGLObjectsManager::CreateIBO() {
		m_Ibos.push_back(Ibo());
		return &m_Ibos.back();
	}

	void OpenGLObjectsManager::ReleaseEverything() {
		for (auto &object : m_Vaos) {
			object.Release();
		}
		for (auto& object : m_Vbos) {
			object.Release();
		}
		for (auto& object : m_Ibos) {
			object.Release();
		}
		m_Vaos.clear();
		m_Vbos.clear();
		m_Ibos.clear();
	}

} }