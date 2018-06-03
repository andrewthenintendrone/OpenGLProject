#pragma once
#include "Texture.h"

class RenderTarget
{
public:

	RenderTarget() {};
	RenderTarget(unsigned int targetCount, unsigned int width, unsigned int height);
	virtual ~RenderTarget();

	bool initialise(unsigned int targetCount, unsigned int width, unsigned int height);

	void bind();
	void unbind();

	unsigned int getWidth() const { return m_width; }
	unsigned int getHeight() const { return m_height; }

	unsigned int getFrameBufferHandle() const { return m_fbo; }

	unsigned int getTargetCount() const { return m_targetCount; }
	const Texture& getTarget(unsigned int target) const { return m_targets[target]; }


protected:

	unsigned int m_width = 0;
	unsigned int m_height = 0;

	unsigned int m_fbo = 0;
	unsigned int m_rbo = 0;

	unsigned int m_targetCount = 0;
	Texture* m_targets = nullptr;
};