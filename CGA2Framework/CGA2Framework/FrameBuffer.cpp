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
	m_samples(0)
{
	if (m_type == FBTYPE::FBT_2D_MULTISAMPLE)
	{
		m_samples = _samples;
	}
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
	m_samples(0)
{
	if (m_type == FBTYPE::FBT_2D_MULTISAMPLE)
	{
		m_samples = _samples;
	}
}


FrameBuffer::~FrameBuffer()
{
	if (m_isallocated)
	{
		if (m_isbound)
		{
			unbind();
		}
		glDeleteFramebuffers(1, &m_fbo); GLERR		
		m_isallocated = false;

		for (auto& att : m_colorbuffers)
		{
			att.second.destroy();
		}

		m_depthbuffer.destroy();
	}
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
		
		unbind();
		
		glDeleteFramebuffers(1, &m_fbo);

		if (checkglerror())
		{
			return false;
		}
		else
		{
			m_isallocated = false;
		}

		for (auto& att : m_colorbuffers)
		{
			att.second.destroy();
		}

		m_depthbuffer.destroy();
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
				Attachment att(m_colorbuffers.size(), tex);
				m_colorbuffers.insert(std::pair<const std::string, Attachment>(_name, att));
				glFramebufferTexture2D(m_glframebuffertarget, GL_COLOR_ATTACHMENT0 + att.aid, GL_TEXTURE_2D, tex->getGLTexture(), 0);
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
				delete tex;
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
				Attachment att(m_colorbuffers.size(), tex);
				m_colorbuffers.insert(std::pair<const std::string, Attachment>(_name, att));
				glFramebufferTexture2D(m_glframebuffertarget, GL_COLOR_ATTACHMENT0 + att.aid, GL_TEXTURE_2D_MULTISAMPLE, tex->getGLTexture(), 0);
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
				delete tex;
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
				Attachment att(m_colorbuffers.size(), tex);
				m_colorbuffers.insert(std::pair<const std::string, Attachment>(_name, att));
				glFramebufferTexture(m_glframebuffertarget, GL_COLOR_ATTACHMENT0 + att.aid, tex->getGLTexture(), 0);
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
				delete tex;
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
			if (tex->buffer(true) && tex->bind())
			{
				m_depthbuffer = Attachment(0, tex);

				GLenum attachment;
				switch (_glinternalformat)
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
					m_depthbuffer = Attachment(0, 0);
					delete tex;
					return false;
					break;
				}

				glFramebufferTexture2D(m_glframebuffertarget, attachment, GL_TEXTURE_2D, m_depthbuffer.tex->getGLTexture(), 0);
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
				delete tex;
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
			if (tex->buffer(true) && tex->bind())
			{
				m_depthbuffer = Attachment(0, tex);

				GLenum attachment;
				switch (_glinternalformat)
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
					m_depthbuffer = Attachment(0, 0);
					delete tex;
					return false;
					break;
				}

				glFramebufferTexture2D(m_glframebuffertarget, attachment, GL_TEXTURE_2D_MULTISAMPLE, m_depthbuffer.tex->getGLTexture(), 0);
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
				delete tex;
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
			if (tex->buffer(true) && tex->bind())
			{
				m_depthbuffer = Attachment(0, tex);

				GLenum attachment;
				switch (_glinternalformat)
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
					m_depthbuffer = Attachment(0, 0);
					delete tex;
					return false;
					break;
				}

				glFramebufferTexture(m_glframebuffertarget, attachment, m_depthbuffer.tex->getGLTexture(), 0);
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
				delete tex;
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

bool FrameBuffer::resolve(FrameBuffer* _msbuffer, FrameBuffer* _resolvebuffer, bool _color, GLint _colorbufferindex, bool _depth, bool _stencil, GLenum _defaultfbcolorbuffer)
{
	if (_msbuffer != nullptr && _msbuffer->m_type == FBTYPE::FBT_2D_MULTISAMPLE)
	{
		if (_resolvebuffer != nullptr)
		{
			GLint sx0, sy0, sx1, sy1, dx0, dy0, dx1, dy1;

			sx0 = _msbuffer->m_vpxoff;
			sy0 = _msbuffer->m_vpyoff;
			sx1 = sx0 + _msbuffer->m_vpwidth;
			sy1 = sy0 + _msbuffer->m_vpheight;

			dx0 = _resolvebuffer->m_vpxoff;
			dy0 = _resolvebuffer->m_vpyoff;
			dx1 = dx0 + _resolvebuffer->m_vpwidth;
			dy1 = dy0 + _resolvebuffer->m_vpheight;

			return blit(_msbuffer, _resolvebuffer, _color, _colorbufferindex, _depth, _stencil,
				sx0, sy0, sx1, sy1,
				dx0, dy0, dx1, dy1);
		}
		else if (_resolvebuffer == nullptr)
		{
			GLint sx0, sy0, sx1, sy1, dx0, dy0, dx1, dy1;

			sx0 = _msbuffer->m_vpxoff;
			sy0 = _msbuffer->m_vpyoff;
			sx1 = sx0 + _msbuffer->m_vpwidth;
			sy1 = sy0 + _msbuffer->m_vpheight;

			GLint curviewport[4];
			glGetIntegerv(GL_VIEWPORT, curviewport); GLERR

			dx0 = curviewport[0];
			dy0 = curviewport[1];
			dx1 = curviewport[0] + curviewport[2];
			dy1 = curviewport[1] + curviewport[3];

			return blit(_msbuffer, _resolvebuffer, _color, _colorbufferindex, _depth, _stencil,
				sx0, sy0, sx1, sy1,
				dx0, dy0, dx1, dy1,
				_defaultfbcolorbuffer);
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

						m_depthbuffer = Attachment(0, rbuf);
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

						m_depthbuffer = Attachment(0, rbuf);
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
						m_depthbuffer = Attachment(0, rbuf);
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
						m_colorbuffers.insert(std::pair<std::string, Attachment>("COLOR", Attachment(0, rbuf)));
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
						m_colorbuffers.insert(std::pair<std::string, Attachment>("COLOR", Attachment(0, rbuf)));
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
						m_colorbuffers.insert(std::pair<std::string, Attachment>("COLOR", Attachment(0, rbuf)));
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
	bool _color,			//blit colorbuffer?
	GLint _colorbufferindex,//what colorbuffer?
	bool _depth,			//blit depthbuffer?
	bool _stencil,			//blit stencilbuffer?
	const GLint _srcx0,
	const GLint _srcy0,
	const GLint _srcx1,
	const GLint _srcy1,
	const GLint _dstx0,
	const GLint _dsty0,
	const GLint _dstx1,
	const GLint _dsty1,
	const GLenum _defaultfbcolorbuffer)
{
	if (_source != nullptr && _target != nullptr)				//FBO -> FBO
	{
		if (_source->isAllocated() && _target->isAllocated())
		{
			if (!_source->bind(FBO_BINDINGMODE::FREAD))
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
				return false;
			}

			if (!_target->bind(FBO_BINDINGMODE::FWRITE))
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
				return false;
			}

			GLenum filter = (_depth || _stencil ? GL_NEAREST : GL_LINEAR);
			GLbitfield mask = 0;
			
			//set read / drawbuffers
			if (_color)
			{
				if (_colorbufferindex >= 0 && _colorbufferindex < _source->m_colorbuffers.size() && _colorbufferindex < _target->m_colorbuffers.size())
				{
					glReadBuffer(GL_COLOR_ATTACHMENT0 + _colorbufferindex); GLERR
					GLenum drawbufs[] = {GL_COLOR_ATTACHMENT0 + _colorbufferindex};
					glDrawBuffers(1,  drawbufs); GLERR
					mask |= GL_COLOR_BUFFER_BIT;
				}
				else
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
					return false;
				}
			}

			if (_depth)
			{
				mask |= GL_DEPTH_BUFFER_BIT;
			}

			if (_stencil)
			{
				mask |= GL_STENCIL_BUFFER_BIT;
			}

			glBlitFramebuffer(
				(_srcx0 == -1 ? _source->m_vpxoff : _srcx0),
				(_srcy0 == -1 ? _source->m_vpyoff : _srcy0),
				(_srcx1 == -1 ? _source->m_vpxoff + _source->m_vpwidth : _srcx1),
				(_srcy1 == -1 ? _source->m_vpyoff + _source->m_vpheight : _srcy1),
				(_dstx0 == -1 ? _target->m_vpxoff : _dstx0),
				(_dsty0 == -1 ? _target->m_vpyoff : _dsty0),
				(_dstx1 == -1 ? _target->m_vpxoff + _target->m_vpwidth : _dstx1),
				(_dsty1 == -1 ? _target->m_vpyoff + _target->m_vpheight : _dsty1),
				mask,
				filter
				);

			if (checkglerror())
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
				return false;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (_source == nullptr && _target != nullptr)		//DefaultFB -> FBO
	{
		if (_target->isAllocated())
		{
			
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); GLERR			

			if (!_target->bind(FBO_BINDINGMODE::FWRITE))
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
				return false;
			}

			GLenum filter = (_depth || _stencil ? GL_NEAREST : GL_LINEAR);
			GLbitfield mask = 0;

			//set read / drawbuffers
			if (_color)
			{
				if (_colorbufferindex < _target->m_colorbuffers.size() && _colorbufferindex >= 0)
				{
					glReadBuffer(_defaultfbcolorbuffer); GLERR
					GLenum drawbufs[] = {GL_COLOR_ATTACHMENT0 + _colorbufferindex};
					glDrawBuffers(1, drawbufs); GLERR
					mask |= GL_COLOR_BUFFER_BIT;
				}
				else
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
					return false;
				}
			}

			if (_depth)
			{
				mask |= GL_DEPTH_BUFFER_BIT;
			}

			if (_stencil)
			{
				mask |= GL_STENCIL_BUFFER_BIT;
			}

			glBlitFramebuffer(
				(_srcx0),
				(_srcy0),
				(_srcx1),
				(_srcy1),
				(_dstx0 == -1 ? _target->m_vpxoff : _dstx0),
				(_dsty0 == -1 ? _target->m_vpyoff : _dsty0),
				(_dstx1 == -1 ? _target->m_vpxoff + _target->m_vpwidth : _dstx1),
				(_dsty1 == -1 ? _target->m_vpyoff + _target->m_vpheight : _dsty1),
				mask,
				filter
				);

			if (checkglerror())
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
				return false;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (_source != nullptr && _target == nullptr)		//FBO -> DefaultFB
	{
		if (_source->isAllocated())
		{
			if (!_source->bind(FBO_BINDINGMODE::FREAD))
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
				return false;
			}
			
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); GLERR			

			GLenum filter = (_depth || _stencil ? GL_NEAREST : GL_LINEAR);
			GLbitfield mask = 0;

			//set read / drawbuffers
			if (_color)
			{
				if (_colorbufferindex >= 0 && _colorbufferindex < _source->m_colorbuffers.size())
				{
					glReadBuffer(GL_COLOR_ATTACHMENT0 + _colorbufferindex); GLERR
					GLenum drawbufs[] = { _defaultfbcolorbuffer };
					glDrawBuffers(1, drawbufs); GLERR
					mask |= GL_COLOR_BUFFER_BIT;
				}
				else
				{
					glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
					return false;
				}
			}

			if (_depth)
			{
				mask |= GL_DEPTH_BUFFER_BIT;
			}

			if (_stencil)
			{
				mask |= GL_STENCIL_BUFFER_BIT;
			}

			glBlitFramebuffer(
				(_srcx0 == -1 ? _source->m_vpxoff : _srcx0),
				(_srcy0 == -1 ? _source->m_vpyoff : _srcy0),
				(_srcx1 == -1 ? _source->m_vpxoff + _source->m_vpwidth : _srcx1),
				(_srcy1 == -1 ? _source->m_vpyoff + _source->m_vpheight : _srcy1),
				(_dstx0),
				(_dsty0),
				(_dstx1),
				(_dsty1),
				mask,
				filter
				);

			if (checkglerror())
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
				return false;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
			return true;
		}
		else
		{
			return false;
		}
	}
	else													//nothing
	{
		return false;
	}
}

bool FrameBuffer::setDrawBuffers()
{
	if (m_glframebuffertarget == GL_FRAMEBUFFER || m_glframebuffertarget == GL_DRAW_FRAMEBUFFER)
	{
		std::vector<GLenum> drawbuffers;
		drawbuffers.resize(m_colorbuffers.size());
		for (auto& att : m_colorbuffers)
		{
			drawbuffers[att.second.aid] = (GL_COLOR_ATTACHMENT0 + att.second.aid);
		}
		glDrawBuffers(drawbuffers.size(), drawbuffers.data());
		return !checkglerror();
	}
	else
	{
		return false;
	}
}