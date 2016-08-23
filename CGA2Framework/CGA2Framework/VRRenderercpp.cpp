#include "VRRenderercpp.h"

bool VRRenderer::Init()
{
	if (!Initialized)
	{
		vr::EVRInitError vrerr;
		vrsys = vr::VR_Init(&vrerr, vr::EVRApplicationType::VRApplication_Scene);
		vrcomp = vr::VRCompositor();
		if (!vrcomp) {
			fprintf(stderr, "OpenVR Compositor initialization failed. See log file for details\n");
			vr::VR_Shutdown();
			return false;
		}

		glGenVertexArrays(1, &quadVAO); GLERR;
		glGenBuffers(1, &quadVBO); GLERR;
		glBindVertexArray(quadVAO); GLERR;
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO); GLERR;
		glBufferData(GL_ARRAY_BUFFER, Primitives::SizeOfQuadVertices, &Primitives::QuadVertices, GL_STATIC_DRAW); GLERR;
		glEnableVertexAttribArray(0); GLERR;
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0); GLERR;
		glEnableVertexAttribArray(1); GLERR;
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); GLERR;
		glBindVertexArray(0); GLERR;
		vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &vrerr);
		vrsys->GetRecommendedRenderTargetSize(&renderWidth, &renderHeight);

		CreateFrameBuffer(renderWidth, renderHeight, leftEyeDesc);
		CreateFrameBuffer(renderWidth, renderHeight, rightEyeDesc);

	}
	return true;
}

void VRRenderer::render(Scene* _scene, RenderFinishedCallback* _callback)
{
	vr::TrackedDevicePose_t pose[vr::k_unMaxTrackedDeviceCount];
	vrcomp->WaitGetPoses(pose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
	glm::mat4 view = _scene->m_camera->GetViewMatrix();

	//Left
	glEnable(GL_MULTISAMPLE); GLERR;
	glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId); GLERR;
	glViewport(0, 0, renderWidth, renderHeight);
	auto proj = convert(vrsys->GetProjectionMatrix(vr::EVREye::Eye_Left, _scene->m_camera->getNear(), _scene->m_camera->getFar(), vr::EGraphicsAPIConvention::API_OpenGL));
	view = view*inverse(mat4(convert(vrsys->GetEyeToHeadTransform(vr::EVREye::Eye_Left))));
	renderer->render(_scene, nullptr, &view, &proj);

	resolveFB(leftEyeDesc.m_nRenderFramebufferId, leftEyeDesc.m_nResolveFramebufferId, renderWidth, renderHeight);
	
	view = _scene->m_camera->GetViewMatrix();

	//Right
	glEnable(GL_MULTISAMPLE); GLERR;
	glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId); GLERR;
	glViewport(0, 0, renderWidth, renderHeight);
	proj = convert(vrsys->GetProjectionMatrix(vr::EVREye::Eye_Right, _scene->m_camera->getNear(), _scene->m_camera->getFar(), vr::EGraphicsAPIConvention::API_OpenGL));
	view = view*inverse(mat4(convert(vrsys->GetEyeToHeadTransform(vr::EVREye::Eye_Right))));
	renderer->render(_scene, nullptr, &view, &proj);

	resolveFB(rightEyeDesc.m_nRenderFramebufferId, rightEyeDesc.m_nResolveFramebufferId, renderWidth, renderHeight);

	vr::Texture_t leftEyeTexture = { (void*)leftEyeDesc.m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);

	vr::Texture_t rightEyeTexture = { (void*)rightEyeDesc.m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	//vrcomp->PostPresentHandoff();


	glViewport(0, 0, _scene->m_height, _scene->m_width);
	glBindVertexArray(0); GLERR;

	glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR;


	

	
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	screenShader->Use();
	glBindVertexArray(quadVAO); GLERR;
	glDisable(GL_DEPTH_TEST); GLERR;
	glBindTexture(GL_TEXTURE_2D, leftEyeDesc.m_nResolveTextureId); GLERR;
	//screenShader->setUniform("screenTexture", leftEyeDesc.m_nResolveTextureId);
	glDrawArrays(GL_TRIANGLES, 0, 6); GLERR;
	glBindVertexArray(0); GLERR;
	_callback->renderFinished();
}