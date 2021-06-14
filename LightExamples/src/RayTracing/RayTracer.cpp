#include "RayTracer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGL/ErrorHandler.h"

#include "Utils.h"

#include <iostream>

RayTracer::RayTracer()
	: m_shader("res/shaders/RayTracing.shader"),
	  m_screenShader("res/shaders/Screen.shader")
{
	setWindowSize(20, 20);
	m_frameBuffer0.bind();
	//m_windowTexture0.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_windowTexture0.getID(), 0);
	//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	//GLCall(glDrawBuffers(1, DrawBuffers));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	m_frameBuffer0.unbind();

	m_frameBuffer1.bind();
	//m_windowTexture1.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_windowTexture1.getID(), 0);
	//glDrawBuffers(1, DrawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	m_frameBuffer1.unbind();
}

RayTracer::~RayTracer()
{
	if (m_bvh != nullptr)
		delete m_bvh;
}

void RayTracer::draw(
	Camera& camera,
	const std::vector<LightSrc>& lightSources,
	const Texture::CubeMap& skyboxCubeMap)
{
	const glm::mat4 newMat = camera.getViewProj();
	if (newMat != m_prevViewProj) {
		resetFrames();
	}
	m_prevViewProj = newMat;

	m_vertexArray.bind();
	m_shader.bind();
	m_shader.setUniform1i("texPosition", 0);
	m_shader.setUniform1i("texNode", 1);
	m_shader.setUniform1i("u_skybox", 2);
	m_shader.setUniform1i("u_normalTexture", 3);
	m_shader.setUniform1i("u_previousTexture", 4);
	m_shader.setUniformMatrix3f("viewToWorld", glm::transpose(camera.getView()));
	m_shader.setUniformVec3f("location", camera.getPos());
	m_shader.setUniform1f("u_fov", camera.getFOV());
	m_shader.setUniformVec2f("screeResolution", glm::vec2(m_width * m_resolutionScale, m_height * m_resolutionScale));
	m_shader.setUniform1i("bvhWidth", m_bvhTexture.getWidth());
	m_shader.setUniform1i("texPosWidth", m_vertexTexture.getWidth());

	const size_t nLightSources = lightSources.size();
	//setupLight
	std::vector<glm::vec3> lightPos;
	lightPos.reserve(32);
	for (size_t i = 0; i < nLightSources; ++i) {
		if (lightSources[i].isEnabled())
			lightPos.push_back(lightSources[i].getPos());
	}
	m_shader.setUniform1i("u_nPointLights", lightPos.size());
	m_shader.setUniformVec3fArray("u_pointLightsPos", lightPos, 32);
	m_shader.setUniform1i("u_nRaysMax", m_nRaysMax);
	m_shader.setUniform1f("u_samplePart", 1.f / m_currentFrame);

	m_vertexTexture.bind(0);
	m_bvhTexture.bind(1);
	skyboxCubeMap.bind(2);
	m_normalTexture.bind(3);
	m_windowTexture1.bind(4);
	// Draw
	if (m_currentFrame % 2 == 1) {
		m_frameBuffer0.bind();
		m_windowTexture1.bind(4);
	}
	else {
		m_frameBuffer1.bind();
		m_windowTexture0.bind(4);
	}

	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, m_width * m_resolutionScale, m_height * m_resolutionScale);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	m_frameBuffer0.unbind();
	m_screenShader.bind();
	m_screenShader.setUniform1i("u_texture", 0);
	if (m_currentFrame % 2 == 1)
		m_windowTexture0.bind(0);
	else
		m_windowTexture1.bind(0);
	glViewport(0, 0, m_width, m_height);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_DEPTH_TEST);
	++m_currentFrame;

	//FOR NOW
	if (m_currentFrame > 1000)
		m_currentFrame = 1000;
}

void RayTracer::updateData(std::vector<Model>& models)
{
	if (!m_isBVHvalid) {
		m_isBVHvalid = true;
		//Create BVH
		if (m_bvh != nullptr)
			delete m_bvh;
		m_bvh = new BVH::BVHBuilder();
		Utils::fillBVH(m_vertexTexture, m_normalTexture, m_bvhTexture, *m_bvh, models);
	}
}

void RayTracer::setWindowSize(const int width, const int height)
{
	m_width = width;
	m_height = height;
	//UPDATE TEXTURE
	m_windowTexture0.loadFromData(0, m_width * m_resolutionScale, m_height * m_resolutionScale);
	m_windowTexture1.loadFromData(0, m_width * m_resolutionScale, m_height * m_resolutionScale);
}

void RayTracer::setInvalid()
{
	m_isBVHvalid = false;
}

int& RayTracer::getMaxRaysRef()
{
	return m_nRaysMax;
}

float RayTracer::getResolutionScale() const
{
	return m_resolutionScale;
}

void RayTracer::setResolutionScale(const float scale)
{
	m_resolutionScale = scale;
	if (m_resolutionScale < 0.1f)
		m_resolutionScale = 0.1f;
	else if (m_resolutionScale > 1.f)
		m_resolutionScale = 1.f;
	//UPDATE TEXTURE
	//m_windowTexture.loadFromData(0, 1, 1);
	m_windowTexture0.loadFromData(0, m_width * m_resolutionScale, m_height * m_resolutionScale);
	m_windowTexture1.loadFromData(0, m_width * m_resolutionScale, m_height * m_resolutionScale);
}

void RayTracer::resetFrames()
{
	m_currentFrame = 1;
	//m_frameBuffer0.bind();
	//glClear(GL_COLOR_BUFFER_BIT);
	//m_frameBuffer1.bind();
	//glClear(GL_COLOR_BUFFER_BIT);
	//m_frameBuffer0.unbind();
}
