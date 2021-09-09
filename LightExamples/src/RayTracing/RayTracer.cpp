#include "RayTracer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "OpenGL/ErrorHandler.h"

#include "Utils.h"

#include <iostream>

RayTracer::RayTracer()
	: m_simpleShader("res/shaders/GroundTruthRayTracing.shader"),
	  m_smartShader("res/shaders/SmartRayTracing.shader"),
	  m_groundTruthShader("res/shaders/GroundTruthRayTracing.shader"),
	  m_screenShader("res/shaders/Screen.shader")
{
	setWindowSize(20, 20);
	m_frameBuffer0.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_windowTexture0.getID(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	m_frameBuffer0.unbind();

	m_frameBuffer1.bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_windowTexture1.getID(), 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	m_frameBuffer1.unbind();

	std::random_device randomDevice = std::random_device();
	m_randomEngine.seed(randomDevice());
	m_distribution = std::uniform_real_distribution<>(0.f, 1.f);
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
	const glm::mat4 newViewProjMat = camera.getViewProj();
	if (newViewProjMat != m_prevViewProj) {
		resetFrames();
	}
	m_prevViewProj = newViewProjMat;

	if (m_currentFrame < m_maxFrames)
		drawFrame(camera, lightSources, skyboxCubeMap);

	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, m_width * m_resolutionScale, m_height * m_resolutionScale);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	m_frameBuffer0.unbind();
	m_screenShader.bind();
	m_screenShader.setUniform1i("u_texture", 0);
	if (m_currentFrame % 2 == 0)
		m_windowTexture0.bind(0);
	else
		m_windowTexture1.bind(0);
	glViewport(0, 0, m_width, m_height);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glEnable(GL_DEPTH_TEST);
}

void RayTracer::updateData(std::vector<Model>& models)
{
	if (!m_isBVHvalid) {
		bool clearBVH = models.empty();
		if (!clearBVH)
			clearBVH = models[0].getLoadableDataRef().objData.getPolygonsCount() == 0;
		if (clearBVH) {
			m_vertexTexture.loadFromData(nullptr, 0, 0);
			m_normalTexture.loadFromData(nullptr, 0, 0);
			m_bvhTexture.loadFromData(nullptr, 0, 0);
			m_isBVHvalid = true;
			delete m_bvh;
			m_bvh = nullptr;
			resetFrames();
			return;
		}
		m_isBVHvalid = true;
		//Create BVH
		if (m_bvh != nullptr)
			delete m_bvh;
		m_bvh = new BVH::BVHBuilder();
		Utils::fillBVH(m_vertexTexture, m_normalTexture, m_bvhTexture, *m_bvh, models);
		resetFrames();
	}
}

void RayTracer::setWindowSize(const int width, const int height)
{
	if ((m_width == width) && (m_height == height))
		return;
	m_width = width;
	m_height = height;
	
	updateTextures();
}

void RayTracer::setInvalid()
{
	m_isBVHvalid = false;
}

int RayTracer::getMaxRays() const
{
	return m_nRaysMax;
}

void RayTracer::setMaxRays(const int value)
{
	if (m_nRaysMax == value)
		return;
	m_nRaysMax = value;
	resetFrames();
}

float RayTracer::getResolutionScale() const
{
	return m_resolutionScale;
}

void RayTracer::setResolutionScale(const float scale)
{
	if (m_resolutionScale == scale)
		return;
	m_resolutionScale = scale;
	if (m_resolutionScale < 0.1f)
		m_resolutionScale = 0.1f;
	else if (m_resolutionScale > 2.f)
		m_resolutionScale = 2.f;

	updateTextures();
}

bool RayTracer::getInterpolateTextures() const
{
	return m_interpolateTextures;
}

void RayTracer::setInterpolateTextures(const bool value)
{
	if (m_interpolateTextures == value)
		return;
	m_interpolateTextures = value;
	m_windowTexture0.bind();
	if (m_interpolateTextures) {
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	else {
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	}

	m_windowTexture1.bind();
	if (m_interpolateTextures) {
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	else {
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	}
}

int RayTracer::getMaxFrames() const
{
	return m_maxFrames;
}

void RayTracer::setMaxFrames(const int value)
{
	if (m_maxFrames == value)
		return;
	if ((m_maxFrames > value) && (m_currentFrame > value))
		resetFrames();
	m_maxFrames = value;
}

void RayTracer::updateTextures()
{
	m_windowTexture0.loadFromData(0, m_width * m_resolutionScale, m_height * m_resolutionScale);
	if (m_interpolateTextures) {
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	m_windowTexture1.loadFromData(0, m_width * m_resolutionScale, m_height * m_resolutionScale);
	if (m_interpolateTextures) {
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}
	resetFrames();
}

void RayTracer::resetFrames()
{
	m_currentFrame = 1;
}

int RayTracer::getCurrentFrame() const
{
	return m_currentFrame;
}

#include "Utils/Console.h"
void RayTracer::drawFrame(
	Camera& camera,
	const std::vector<LightSrc>& lightSources,
	const Texture::CubeMap& skyboxCubeMap)
{
	Shader& shader = (m_type == Type::GROUND_TRUTH ? m_groundTruthShader : (m_type == Type::SMART ? m_smartShader : m_simpleShader));
	m_vertexArray.bind();
	shader.bind();
	shader.setUniform1i("texPosition", 0);
	shader.setUniform1i("texNode", 1);
	shader.setUniform1i("u_skybox", 2);
	shader.setUniform1i("u_normalTexture", 3);
	shader.setUniform1i("u_previousTexture", 4);
	shader.setUniformMatrix3f("viewToWorld", glm::transpose(camera.getView()));
	shader.setUniformVec3f("location", camera.getPos());
	shader.setUniform1f("u_fov", camera.getFOV());
	shader.setUniformVec2f("screenResolution", glm::vec2(m_width * m_resolutionScale, m_height * m_resolutionScale));
	shader.setUniform1i("bvhWidth", m_bvhTexture.getWidth());
	shader.setUniform1i("texPosWidth", m_vertexTexture.getWidth());

	const size_t nLightSources = lightSources.size();
	//setupLight
	std::vector<glm::vec3> lightPos;
	std::vector<float> lightScale;
	lightPos.reserve(32);
	lightScale.reserve(32);
	for (size_t i = 0; i < nLightSources; ++i) {
		if (lightSources[i].isEnabled()) {
			lightPos.push_back(lightSources[i].getPos());
			lightScale.push_back(lightSources[i].getScale());
		}
	}
	shader.setUniform1i("u_nPointLights", lightPos.size());
	shader.setUniformVec3fArray("u_pointLightsPos", lightPos, 32);
	shader.setUniform1fArray("u_pointLightsScale", lightScale, 32);

	shader.setUniform1i("u_nRaysMax", m_nRaysMax);
	shader.setUniform1f("u_samplePart", 1.f / m_currentFrame);
	Console::print(std::string("Sample part: ") + std::to_string(1.f / m_currentFrame) + "; Frame: " + std::to_string(m_currentFrame) + "\n");

	shader.setUniformVec2f("u_seed1", glm::vec2((float)m_distribution(m_randomEngine), (float)m_distribution(m_randomEngine)) * 999.f);
	shader.setUniformVec2f("u_seed2", glm::vec2((float)m_distribution(m_randomEngine), (float)m_distribution(m_randomEngine)) * 999.f);

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

	++m_currentFrame;
}
