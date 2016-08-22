#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(const FBTYPE _type, const GLint _samples) :
	m_fbo(0),
	m_colorbuffers(),
	m_depthbuffer(0,0),
	m_vpwidth(0),
	m_vpheight(0),
	m_vpxoff(0),
	m_vpyoff(0),
	m_isallocated(false),
	m_isbound(false),
	m_type(_type),
	m_samples(_samples)
{
}

FrameBuffer::FrameBuffer(const GLint _vpxoff, const GLint _vpyoff, const GLint _vpwidth, const GLint _vpheight, const FBTYPE _type, const GLint _samples) :
	m_fbo(0),
	m_colorbuffers(),
	m_depthbuffer(0,0),
	m_vpwidth(_vpwidth),
	m_vpheight(_vpheight),
	m_vpxoff(_vpxoff),
	m_vpyoff(_vpyoff),
	m_isallocated(false),
	m_isbound(false),
	m_type(_type),
	m_samples(_samples)
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

Texture* FrameBuffer::getColorBufferTex(const std::string& _name)
{
	try
	{
		return m_colorbuffers.at(_name).tex;
	}
	catch (std::out_of_range ex)
	{
		return nullptr;
	}
}

Texture* FrameBuffer::getDepthBufferTex()
{
	return m_depthbuffer.tex;
}

//TODO: Bind attachments to framebuffer, check if fbo is allocated and bound before doing this!
bool FrameBuffer::addColorBufferTex(
	const std::string& _name,
	GLint _glinternalformat,
	GLenum _glformat,
	GLenum _gltype)
{
	if (!m_isallocated || !m_isbound)
	{
		std::cout << "FRAMEBUFFER ERROR: Framebuffer is not bound or allocated. Allocate and bind the framebuffer before adding render textures!";
		return false;
	}

	if (m_colorbuffers.find(_name) != m_colorbuffers.end())
	{
		return false;
	}

	switch (m_type)
	{
		case FBTYPE::FBT_2D:
		{
			Texture2D* tex = new Texture2D(m_vpwidth, m_vpheight, _name);
			tex->setGLInternalFormat(_glinternalformat);
			tex->setGLFormat(_glformat);
			tex->setGLType(_gltype);
			tex->setBindingOptions(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			if (tex->buffer(true))
			{
				m_colorbuffers.insert(std::pair<const std::string, Attachment>(_name, Attachment(m_colorbuffers.size(), tex)));
				glFramebufferTexture2D(m_glframebuffertarget, GL_COLOR_ATTACHMENT0 + m_colorbuffers.size() - 1, GL_TEXTURE_2D, tex->getGLTexture(), 0);
				if (checkglerror())
				{
					if (m_colorbuffers[_name].tex != nullptr)
					{
						delete m_colorbuffers[_name].tex;
						m_colorbuffers[_name].tex = nullptr;
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
			break;
		}
		case FBTYPE::FBT_2D_MULTISAMPLE:
		{
			Texture2D* tex = new Texture2D(m_vpwidth, m_vpheight, _name);
			tex->setGLInternalFormat(_glinternalformat);
			tex->setGLFormat(_glformat);
			tex->setGLType(_gltype);
			tex->setBindingOptions(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			tex->setMultisampling(true, m_samples);
			if (tex->buffer(true))
			{
				m_colorbuffers.insert(std::pair<const std::string, Attachment>(_name, Attachment(m_colorbuffers.size(), tex)));
				glFramebufferTexture2D(m_glframebuffertarget, GL_COLOR_ATTACHMENT0 + m_colorbuffers.size() - 1, GL_TEXTURE_2D_MULTISAMPLE, tex->getGLTexture(), 0);
				if (checkglerror())
				{
					if (m_colorbuffers[_name].tex != nullptr)
					{
						delete m_colorbuffers[_name].tex;
						m_colorbuffers[_name].tex = nullptr;
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
			break;
		}
		case FBTYPE::FBT_CUBEMAP:
		{
			TextureCB* tex = new TextureCB(m_vpwidth, _name);
			tex->setGLInternalFormat(_glinternalformat);
			tex->setGLFormat(_glformat);
			tex->setGLType(_gltype);
			tex->setBindingOptions(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			if (tex->buffer(true))
			{
				m_colorbuffers.insert(std::pair<const std::string, Attachment>(_name, Attachment(m_colorbuffers.size(), tex)));
				glFramebufferTexture(m_glframebuffertarget, GL_COLOR_ATTACHMENT0 + m_colorbuffers.size() - 1, tex->getGLTexture(), 0);
				if (checkglerror())
				{
					if (m_colorbuffers[_name].tex != nullptr)
					{
						delete m_colorbuffers[_name].tex;
						m_colorbuffers[_name].tex = nullptr;
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
			break;
		}
		default:
		{
			return false;
			break;
		}			
	}
}

//TODO: Bind attachments to framebuffer, check if fbo is allocated and bound before doing this!
bool FrameBuffer::setDepthBufferTex(
	GLint _glinternalformat,
	GLenum _glformat,
	GLenum _gltype
	)
{

	if (!m_isallocated || !m_isbound)
	{
		std::cout << "FRAMEBUFFER ERROR: Framebuffer is not bound or allocated. Allocate and bind the framebuffer before adding render textures!";
		return false;
	}

	if (m_depthbuffer.tex != nullptr)
	{
		delete m_depthbuffer.tex;
		m_depthbuffer.tex = nullptr;
	}

	if (m_depthbuffer.renderbufferhandle != 0)
	{
		glDeleteRenderbuffers(1, &m_depthbuffer.renderbufferhandle); GLERR
	}

	switch (m_type)
	{
		case FBTYPE::FBT_2D:
		{
			Texture2D* tex = new Texture2D(m_vpwidth, m_vpheight,"");
			tex->setGLInternalFormat(_glinternalformat);
			tex->setGLFormat(_glformat);
			tex->setGLType(_gltype);
			tex->setBindingOptions(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			if (tex->buffer(true))
			{
				m_depthbuffer = Attachment(0, tex);
				glFramebufferTexture2D(m_glframebuffertarget, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthbuffer.tex->getGLTexture(), 0);
				if (checkglerror())
				{
					if (m_depthbuffer.tex != nullptr)
					{
						delete m_depthbuffer.tex;
						m_depthbuffer.tex = nullptr;
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
			break;
		}
		case FBTYPE::FBT_2D_MULTISAMPLE:
		{
			Texture2D* tex = new Texture2D(m_vpwidth, m_vpheight,"");
			tex->setGLInternalFormat(_glinternalformat);
			tex->setGLFormat(_glformat);
			tex->setGLType(_gltype);
			tex->setBindingOptions(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			tex->setMultisampling(true, m_samples);
			if (tex->buffer(true))
			{
				m_depthbuffer = Attachment(0, tex);
				glFramebufferTexture2D(m_glframebuffertarget, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, m_depthbuffer.tex->getGLTexture(), 0);
				if (checkglerror())
				{
					if (m_depthbuffer.tex != nullptr)
					{
						delete m_depthbuffer.tex;
						m_depthbuffer.tex = nullptr;
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
			break;
		}
		case FBTYPE::FBT_CUBEMAP:
		{
			TextureCB* tex = new TextureCB(m_vpwidth,"");
			tex->setGLInternalFormat(_glinternalformat);
			tex->setGLFormat(_glformat);
			tex->setGLType(_gltype);
			tex->setBindingOptions(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
			if (tex->buffer(true))
			{
				m_depthbuffer = Attachment(0, tex);
				glFramebufferTexture(m_glframebuffertarget, GL_DEPTH_ATTACHMENT, m_depthbuffer.tex->getGLTexture(), 0);
				if (checkglerror())
				{
					if (m_depthbuffer.tex != nullptr)
					{
						delete m_depthbuffer.tex;
						m_depthbuffer.tex = nullptr;
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
			break;
		}
		default:
		{
			return false;
			break;
		}
	}
}

//TODO: Create renderbuffers and bind them, check if fbo is allocated and bound before doing this!
//bool FrameBuffer::addColorRenderBuffer(
//	const std::string& _name,
//	GLint _glinternalformat,
//	GLenum _glformat,
//	GLenum _gltype)
//{
//	switch (m_type)
//	{
//	case FBTYPE::FBT_2D:
//		break;
//	case FBTYPE::FBT_2D_MULTISAMPLE:
//		break;
//	case FBTYPE::FBT_CUBEMAP:
//		break;
//	default:
//		return false;
//		break;
//	}
//}

//TODO: Create renderbuffers and bind them, check if fbo is allocated and bound before doing this!
//bool FrameBuffer::setDepthRenderBuffer(
//	GLint _glinternalformat,
//	GLenum _glformat,
//	GLenum _gltype
//	)
//{
//	return false;
//}

bool FrameBuffer::resolve(FrameBuffer* _resolvebuffer)
{
	return false;
}

bool FrameBuffer::complete(GLenum _depthinternalformat, GLenum _colorinternalformat)
{
	if (m_isbound && m_isallocated)
	{
		//create depth renderbuffer if needed
		if (m_depthbuffer.tex == nullptr && m_depthbuffer.renderbufferhandle == 0)
		{
			GLuint rbuf;
			glGenRenderbuffers(1, &rbuf);
			if (rbuf != 0 && !checkglerror())
			{
				glBindRenderbuffer(GL_RENDERBUFFER, rbuf);
				if (checkglerror())
				{
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					glDeleteRenderbuffers(1, &rbuf);
					return false;
				}
				
				switch (m_type)
				{
					case FBTYPE::FBT_2D:
					{
						glRenderbufferStorage(GL_RENDERBUFFER, _depthinternalformat, m_vpwidth, m_vpheight);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}

						glBindRenderbuffer(GL_RENDERBUFFER, 0);

						GLenum attachment;
						switch (_depthinternalformat)
						{
						case GL_DEPTH_COMPONENT16:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT24:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT32:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT32F:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH24_STENCIL8:
							attachment = GL_DEPTH_STENCIL_ATTACHMENT;
							break;
						case GL_DEPTH32F_STENCIL8:
							attachment = GL_DEPTH_STENCIL_ATTACHMENT;
							break;
						default:
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
							break;
						}						

						glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbuf);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}						
						break;
					}
					case FBTYPE::FBT_2D_MULTISAMPLE:
					{
						glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples ,_depthinternalformat, m_vpwidth, m_vpheight);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}

						glBindRenderbuffer(GL_RENDERBUFFER, 0);

						GLenum attachment;
						switch (_depthinternalformat)
						{
						case GL_DEPTH_COMPONENT16:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT24:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT32:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT32F:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH24_STENCIL8:
							attachment = GL_DEPTH_STENCIL_ATTACHMENT;
							break;
						case GL_DEPTH32F_STENCIL8:
							attachment = GL_DEPTH_STENCIL_ATTACHMENT;
							break;
						default:
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
							break;
						}

						glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbuf);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}
						break;
					}
					case FBTYPE::FBT_CUBEMAP:
					{
						glRenderbufferStorage(GL_RENDERBUFFER, _depthinternalformat, m_vpwidth, m_vpheight);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}

						glBindRenderbuffer(GL_RENDERBUFFER, 0);

						GLenum attachment;
						switch (_depthinternalformat)
						{
						case GL_DEPTH_COMPONENT16:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT24:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT32:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH_COMPONENT32F:
							attachment = GL_DEPTH_ATTACHMENT;
							break;
						case GL_DEPTH24_STENCIL8:
							attachment = GL_DEPTH_STENCIL_ATTACHMENT;
							break;
						case GL_DEPTH32F_STENCIL8:
							attachment = GL_DEPTH_STENCIL_ATTACHMENT;
							break;
						default:
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
							break;
						}

						glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbuf);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}
						break;
					}
					default:
					{
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
						glDeleteRenderbuffers(1, &rbuf);
						return false;
						break;
					}
				}
			}
			else
			{
				return false;
			}
		}

		//create color renderbuffer if needed
		if (m_colorbuffers.size() == 0)
		{
			GLuint rbuf;
			glGenRenderbuffers(1, &rbuf);
			if (rbuf != 0 && !checkglerror())
			{
				glBindRenderbuffer(GL_RENDERBUFFER, rbuf);
				if (checkglerror())
				{
					glBindRenderbuffer(GL_RENDERBUFFER, 0);
					glDeleteRenderbuffers(1, &rbuf);
					return false;
				}

				switch (m_type)
				{
					case FBTYPE::FBT_2D:
					{
						glRenderbufferStorage(GL_RENDERBUFFER, _colorinternalformat, m_vpwidth, m_vpheight);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}

						glBindRenderbuffer(GL_RENDERBUFFER, 0);

						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbuf);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}
						break;
					}
					case FBTYPE::FBT_2D_MULTISAMPLE:
					{
						glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, _colorinternalformat, m_vpwidth, m_vpheight);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}

						glBindRenderbuffer(GL_RENDERBUFFER, 0);

						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbuf);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}
						break;
					}
					case FBTYPE::FBT_CUBEMAP:
					{
						glRenderbufferStorage(GL_RENDERBUFFER, _colorinternalformat, m_vpwidth, m_vpheight);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}

						glBindRenderbuffer(GL_RENDERBUFFER, 0);

						glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbuf);

						if (checkglerror())
						{
							glBindRenderbuffer(GL_RENDERBUFFER, 0);
							glDeleteRenderbuffers(1, &rbuf);
							return false;
						}
						break;
					}
					default:
					{
						glBindRenderbuffer(GL_RENDERBUFFER, 0);
						glDeleteRenderbuffers(1, &rbuf);
						return false;
						break;
					}
				}
			}
			else
			{
				return false;
			}
		}

		if (isComplete())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
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
				std::cout << "FRAMEBUFFER ERROR: The image formats specified are not supported by the system's OpenGL implementation." << std::endl;
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

bool FrameBuffer::blit(FrameBuffer* _source,
	FrameBuffer* _target,
	const GLint _xoff,
	const GLint _yoff,
	const GLint _width,
	const GLint _height,
	bool colorbit,
	GLint _colorbufferindex,
	bool _depthbit,
	bool _stencilbit)
{

}

bool FrameBuffer::blitdefault(bool _frontback,
	FrameBuffer* _target,
	const GLint _xoff,
	const GLint _yoff,
	const GLint _width,
	const GLint _height,
	bool _depthbit,
	bool _stencilbit)
{

}