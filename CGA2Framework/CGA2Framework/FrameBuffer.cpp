#include "FrameBuffer.h"


FrameBuffer::FrameBuffer() :
	m_fbo(0),
	m_colorrenderbuffers(),
	m_depthrenderbuffer(0),
	m_colorbuffers(),
	m_depthbuffer(0),
	m_vpwidth(0),
	m_vpheight(0),
	m_vpxoff(0),
	m_vpyoff(0),
	m_isallocated(false),
	m_isbound(false)
{
}

FrameBuffer::FrameBuffer(const GLint _vpwidth, const GLint _vpheight, const GLint _vpxoff, const GLint _vpyoff) :
	m_fbo(0),
	m_colorrenderbuffers(),
	m_depthrenderbuffer(0),
	m_colorbuffers(),
	m_depthbuffer(0),
	m_vpwidth(_vpwidth),
	m_vpheight(_vpheight),
	m_vpxoff(_vpxoff),
	m_vpyoff(_vpyoff),
	m_isallocated(false),
	m_isbound(false)
{
}


FrameBuffer::~FrameBuffer()
{
}

bool FrameBuffer::allocate()
{
	if (m_fbo == 0 && !m_isallocated)
	{
		glGenFramebuffers(1, &m_fbo);
		if (checkglerror() || m_fbo == 0)
		{
			std::cout << "Error creating framebuffer object." << std::endl;
			m_fbo = 0;
			m_isallocated = false;
			return false;
		}
		else
		{
			m_isallocated = true;
			return true;
		}
	}
	else
	{
		std::cout << "The framebuffer is already allocated." << std::endl;
		return false;
	}
}

bool FrameBuffer::destroy()
{
	if (m_isallocated)
	{
		if (m_isbound)
		{
			unbind();
		}
		glDeleteFramebuffers(1, &m_fbo);

		if (checkglerror())
		{
			return false;
		}
		else
		{
			m_isallocated = false;
			return true;
		}
	}
	else
	{
		std::cout << "The framebuffer you want to delete is not allocated." << std::endl;
		return false;
	}
}

bool FrameBuffer::bind(const FBO_BINDINGMODE _mode)
{
	if (m_isallocated && m_fbo != 0)
	{
		switch (_mode)
		{
		case FREAD:
			glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
			if (checkglerror())
			{
				m_isbound = false;
				return false;
			}
			else
			{
				m_isbound = true;
				m_glframebuffertarget = GL_READ_FRAMEBUFFER;
				return true;
			}
			break;
		case FWRITE:
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
			if (checkglerror())
			{
				m_isbound = false;
				return false;
			}
			else
			{
				m_isbound = true;
				m_glframebuffertarget = GL_DRAW_FRAMEBUFFER;
				return true;
			}
			break;
		case FREADWRITE:
			glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
			if (checkglerror())
			{
				m_isbound = false;
				return false;
			}
			else
			{
				m_isbound = true;
				m_glframebuffertarget = GL_FRAMEBUFFER;
				return true;
			}
			break;
		default:
			std::cout << "Unknown framebuffer binding mode. Please pass a valid value from the FB_BINDINGMODE enumeration." << std::endl;
			m_isbound = false;
			return false;
			break;
		}
	}
	else
	{
		std::cout << "The framebuffer is not allocated yet." << std::endl;
		return false;
	}
}

bool FrameBuffer::unbind()
{
	if (m_isbound)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (checkglerror())
		{
			return false;
		}
		else
		{
			m_isbound = false;
			m_glframebuffertarget = GL_INVALID_ENUM;
			return true;
		}
	}
	else
	{
		std::cout << "The framebuffer is not bound." << std::endl;
		return false;
	}
}

bool FrameBuffer::updateGLViewport()
{
	if (m_vpwidth >= 0 && m_vpheight >= 0 && m_vpxoff >= 0 && m_vpyoff >= 0)
	{
		glViewport(m_vpxoff, m_vpyoff, m_vpwidth, m_vpheight); GLERR
		return true;
	}
	else
	{
		std::cout << "Invalid viewport values. The values must be non-negative." << std::endl;
		return false;
	}
}

bool FrameBuffer::updateGLViewport(const GLint _vpxoff, const GLint _vpyoff, const GLint _vpwidth, const GLint _vpheight)
{
	if (_vpwidth >= 0 && _vpheight >= 0 && _vpxoff >= 0 && _vpyoff >= 0)
	{
		m_vpxoff = _vpxoff;
		m_vpyoff = _vpyoff;
		m_vpwidth = _vpwidth;
		m_vpheight = _vpheight;
		glViewport(m_vpxoff, m_vpyoff, m_vpwidth, m_vpheight); GLERR
		return true;
	}
	else
	{
		std::cout << "Invalid viewport values. The values must be non-negative." << std::endl;
		return false;
	}
}

bool FrameBuffer::blit(FrameBuffer* _target)
{

}

Texture* FrameBuffer::getColorBuffer(const std::string& _name)
{
	return m_colorbuffers.find(_name)->second;
}

Texture* FrameBuffer::getDepthBuffer()
{
	return m_depthbuffer;
}

bool FrameBuffer::addColorBuffer2D(
	const std::string& _name,
	GLint _glinternalformat,
	GLenum _glformat,
	GLenum _gltype,
	GLboolean _multisample,
	GLint _samples)
{

}

bool FrameBuffer::setDepthBuffer2D(
	GLint _glinternalformat,
	GLenum _glformat,
	GLenum _gltype,
	GLboolean _multisample,
	GLint _samples
	)
{

}

bool FrameBuffer::addColorBufferCB(
	const std::string& _name,
	GLint _glinternalformat,
	GLenum _glformat,
	GLenum _gltype)
{

}

bool FrameBuffer::setDepthBufferCB(
	GLint _glinternalformat,
	GLenum _glformat,
	GLenum _gltype)
{

}

bool FrameBuffer::resolve(FrameBuffer* _resolvebuffer)
{

}

bool FrameBuffer::complete()
{

}

bool FrameBuffer::checkfbostate()
{
	if (m_isallocated && m_isbound && m_fbo != 0)
	{
		if (m_glframebuffertarget == GL_DRAW_FRAMEBUFFER || m_glframebuffertarget == GL_READ_FRAMEBUFFER || m_glframebuffertarget == GL_FRAMEBUFFER)
		{
			GLenum res = glCheckFramebufferStatus(m_glframebuffertarget); GLERR
			switch (res)
			{
			case GL_FRAMEBUFFER_COMPLETE:
				return true;
				break;
			case GL_FRAMEBUFFER_UNDEFINED:
				std::cout << "FRAMEBUFFER ERROR: The default framebuffer is undefined." << std::endl;
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
				std::cout << "FRAMEBUFFER ERROR: Framebuffer attachments incomplete." << std::endl;
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				std::cout << "FRAMEBUFFER ERROR: There must be at least one colorbuffer attached to the framebuffer." << std::endl;
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
				std::cout << "FRAMEBUFFER ERROR: Incomplete draw buffer: Invalid GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE." << std::endl;
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
				std::cout << "FRAMEBUFFER ERROR: Incomplete read buffer: Invalid GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE." << std::endl;
				return false;
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED:
				std::cout << "FRAMEBUFFER ERROR: The image formats you specified are not supported by the system's OpenGL implementation." << std::endl;
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
				std::cout << "FRAMEBUFFER ERROR: All images must have the same number of multisample samples." << std::endl;
				return false;
				break;
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
				std::cout << "FRAMEBUFFER ERROR: There is a layered image attached, but other attachments aren't layered. The attachments must be consistent." << std::endl;
				return false;
				break;
			default:
				std::cout << "FRAMEBUFFER ERROR: Unknown error. The framebuffer is incomplete." << std::endl;
				return false;
				break;
			}
		}
		else
		{
			std::cout << "Error. Invalid framebuffer target." << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "The framebuffer isn't bound or allocated." << std::endl;
		return false;
	}
}
