#pragma once
#include "SceneRenderer.h"
#include "FrameBuffer.h"
#include "Primitives.h"
#include <openvr.h>
#include "glerror.h"
class VRRenderer :
	public SceneRenderer
{
public:
	//VRRenderer(){};
	VRRenderer(SceneRenderer* _renderer, Shader *_shader){ renderer = _renderer; screenShader = _shader; Init(); };
	~VRRenderer(){
		vr::VR_Shutdown(); glDeleteVertexArrays(1, &quadVAO);
		glDeleteBuffers(1, &quadVBO);
	};

	bool Init();
	void render(Scene* _scene, RenderFinishedCallback* _callback) override;
	void setRenderer(SceneRenderer* _renderer){ renderer = _renderer; }
	void setScreenShader(Shader* _shader){ screenShader = _shader; }

	//Replace with own FBO class later
	struct FramebufferDesc
	{
		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;
		GLuint m_nRenderFramebufferId;
		GLuint m_nResolveTextureId;
		GLuint m_nResolveFramebufferId;
	};

private:
	SceneRenderer* renderer;
	Shader* screenShader;
	uint32 renderHeight, renderWidth;

	GLuint quadVAO, quadVBO;

	FramebufferDesc leftEyeDesc;
	FramebufferDesc rightEyeDesc;

	/*FrameBuffer leftEye;
	FrameBuffer leftResolve;
	FrameBuffer rightEye;
	FrameBuffer rightResolve;*/
	vr::IVRSystem* vrsys;
	vr::IVRCompositor* vrcomp;

	bool Initialized=false;
	bool CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc &framebufferDesc)
	{
		//create multisample framebuffer
		glGenFramebuffers(1, &framebufferDesc.m_nRenderFramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nRenderFramebufferId);

		glGenRenderbuffers(1, &framebufferDesc.m_nDepthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, nWidth, nHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);

		glGenTextures(1, &framebufferDesc.m_nRenderTextureId);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, nWidth, nHeight, true);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId, 0);

		//create resolve framebuffer as intermediate to get a normal 2d texture from multisampled buffer for submitting to the HMD and rendering onto window quad
		glGenFramebuffers(1, &framebufferDesc.m_nResolveFramebufferId);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nResolveFramebufferId);

		glGenTextures(1, &framebufferDesc.m_nResolveTextureId);
		glBindTexture(GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId, 0);

		// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}
};
