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

		vrcomp->WaitGetPoses(lastposes, vr::k_unMaxTrackedDeviceCount, NULL, 0);

		CreateFrameBuffer(renderWidth, renderHeight, leftEyeDesc);
		CreateFrameBuffer(renderWidth, renderHeight, rightEyeDesc);

	}
	return true;
}

void VRRenderer::render(Scene* _scene, RenderFinishedCallback* _callback)
{

	if (!_scene->m_camera->getTransform().isKeptOverriden())
		_scene->m_camera->getTransform().keepOverriden(true);

	auto trans = _scene->m_camera->getTransform().getTranslate();
	auto hmdView = mat4(convert(lastposes[3].mDeviceToAbsoluteTracking));

	hmdView[3][0] += trans.x;
	hmdView[3][1] += trans.y;
	hmdView[3][2] += trans.z;

	hmdView = inverse(hmdView);

	_scene->m_camera->getTransform().setMatrix(hmdView);

	renderer->AdjustViewport(renderWidth, renderHeight);

	//Left
	glEnable(GL_MULTISAMPLE); GLERR;
	glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId); GLERR;
	glViewport(0, 0, renderWidth, renderHeight);
	auto proj = convert(vrsys->GetProjectionMatrix(vr::EVREye::Eye_Left, _scene->m_camera->getNear(), _scene->m_camera->getFar(), vr::EGraphicsAPIConvention::API_OpenGL));
	auto tview = hmdView*inverse(mat4(convert(vrsys->GetEyeToHeadTransform(vr::EVREye::Eye_Left))));
	renderer->render(_scene, nullptr, &tview, &proj);

	resolveFB(leftEyeDesc.m_nRenderFramebufferId, leftEyeDesc.m_nResolveFramebufferId, renderWidth, renderHeight);
	
	//tview = _scene->m_camera->GetViewMatrix();

	//Right
	glEnable(GL_MULTISAMPLE); GLERR;
	glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId); GLERR;
	glViewport(0, 0, renderWidth, renderHeight);
	proj = convert(vrsys->GetProjectionMatrix(vr::EVREye::Eye_Right, _scene->m_camera->getNear(), _scene->m_camera->getFar(), vr::EGraphicsAPIConvention::API_OpenGL));
	tview = hmdView*inverse(mat4(convert(vrsys->GetEyeToHeadTransform(vr::EVREye::Eye_Right))));
	renderer->render(_scene, nullptr, &tview, &proj);

	resolveFB(rightEyeDesc.m_nRenderFramebufferId, rightEyeDesc.m_nResolveFramebufferId, renderWidth, renderHeight);

	vr::Texture_t leftEyeTexture = { (void*)leftEyeDesc.m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);

	vr::Texture_t rightEyeTexture = { (void*)rightEyeDesc.m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	//vrcomp->PostPresentHandoff();

	vr::TrackedDevicePose_t pose[vr::k_unMaxTrackedDeviceCount];
	vrcomp->WaitGetPoses(pose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	float fSecondsSinceLastVsync;
	vrsys->GetTimeSinceLastVsync(&fSecondsSinceLastVsync, NULL);

	float fDisplayFrequency = vrsys->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);
	float fFrameDuration = 1.f / fDisplayFrequency;
	float fVsyncToPhotons = vrsys->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float);

	float fPredictedSecondsFromNow = fFrameDuration - fSecondsSinceLastVsync + fVsyncToPhotons;
	
	vrsys->GetDeviceToAbsoluteTrackingPose(vr::ETrackingUniverseOrigin::TrackingUniverseStanding, fPredictedSecondsFromNow,lastposes, vr::k_unMaxTrackedDeviceCount);


	glBindVertexArray(0); GLERR;

	glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR;


	glViewport(0, 0, _scene->m_width, _scene->m_height);

	
	
	glClearColor(1.0f, 5.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	
	glDisable(GL_DEPTH_TEST); GLERR;
	screenShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, leftEyeDesc.m_nResolveTextureId); GLERR;
	screenShader->setUniform("screenTexture", 0);
	//screenShader->setUniform("screenTexture", leftEyeDesc.m_nResolveTextureId);
	Primitives::drawNDCQuad();
	_callback->renderFinished();
}

