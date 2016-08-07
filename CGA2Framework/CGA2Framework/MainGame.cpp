#include "MainGame.h"
namespace {
glm::mat4 convert(const vr::HmdMatrix44_t &m) {
	return glm::mat4(
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}

glm::mat4x3 convert(const vr::HmdMatrix34_t &m) {
	return glm::mat4x3(
		m.m[0][0], m.m[1][0], m.m[2][0],
		m.m[0][1], m.m[1][1], m.m[2][1],
		m.m[0][2], m.m[1][2], m.m[2][2],
		m.m[0][3], m.m[1][3], m.m[2][3]);
}}
const GLfloat MainGame::vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // Positions   // TexCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

quat modelOrientation(vec3(0.0f,0,0));
vec2 mpos(400,300);
bool discall=false;
GLuint quadVAO, quadVBO;
MainGame::MainGame(const GLint sizex, const GLint sizey, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLboolean uselatestglver)
	: GameWindow(sizex, sizey, cvmaj, cvmin, title, uselatestglver)
{


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	

	glBindVertexArray(0); // Unbind VAO
	
	glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glBindVertexArray(0);

	cam = new FPSCamera((GLfloat)45.0f, WIDTH(), HEIGHT(), (GLfloat)0.1f, (GLfloat)100.0f, vec3(0,1,0), vec3(1,0,0), vec3(0,0,1));
	vr::EVRInitError vrerr;
	vrsys = vr::VR_Init(&vrerr,vr::EVRApplicationType::VRApplication_Scene); 
	vr::VRCompositor();
	vrsys->GetRecommendedRenderTargetSize(&renderwidth, &renderheight);
	CreateFrameBuffer(renderwidth, renderheight, leftEyeDesc);
	CreateFrameBuffer(renderwidth, renderheight, rightEyeDesc);
	shader = new Shader("Sample.vs", "Sample.fs");
	
	screenshader = new Shader("quad.vs", "quad.fs");
	for(auto begin = std::begin(keys), end = std::end(keys); begin!=end; ++begin) *begin = false;
	 m_VA =0.0f;
	 m_HA=0.0f;
}


MainGame::~MainGame()
{
	vr::VR_Shutdown();
	delete vrsys;
	delete cam;
	delete shader;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void MainGame::keycallback(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void MainGame::mmcallback(double xpos, double ypos)
{
	mpos = vec2(xpos, ypos);

	//fprintf(stderr, "Mousepos: X: %.0f Y: %.0f\n", xpos, ypos);
}
void MainGame::mbcallback(int button, int action, int mods)
{

}

void MainGame::mscrcallback(double xoffset, double yoffset)
{

}

GLvoid MainGame::update(GLdouble time)
{
	MoveData md= {vec3(0,0,0),0};
	
	md.Multiplier = 0.05f;
	if (keys[GLFW_KEY_W])
		md.mtype += vec3(1,0,0);
	if (keys[GLFW_KEY_S])
		md.mtype += vec3(-1,0,0);
	if (keys[GLFW_KEY_A])
		md.mtype += vec3(0,0,1);
	if (keys[GLFW_KEY_D])
		md.mtype += vec3(0,0,-1);
	if (keys[GLFW_KEY_SPACE])
		modelOrientation = quat(vec3(0,0,0));
	vec3 EulerAngles(0, 0, 0);
	if(keys[GLFW_KEY_RIGHT])
	{
		EulerAngles+=vec3(0,0.02f,0);
		//modelOrientation = normalize(modelOrientation)*quat(EulerAngles);
	}
	if(keys[GLFW_KEY_LEFT])
	{
		EulerAngles +=vec3(0.02f,0,0);
		//modelOrientation = normalize(modelOrientation)*quat(EulerAngles);
	}
	if(keys[GLFW_KEY_UP])
	{
		EulerAngles += vec3(0,0,0.02f);
		//modelOrientation = normalize(modelOrientation)*quat(EulerAngles);
	}
	//modelOrientation = normalize(modelOrientation)*quat(EulerAngles);
	if(md.mtype!=vec3(0,0,0))
		cam->Move(md);
	vec2 delta(mpos-vec2(400,300));
	delta = delta*0.02f;
	if(delta!=vec2(0,0)&& keys[GLFW_KEY_M])
	{
		vec3 EulerAnglesM(delta.x,delta.y, 0);
		modelOrientation = RotateQuat(EulerAngles);
		//modelOrientation = quat(EulerAngles);
		cam->Rotate(EulerAnglesM);
	}
	delta = vec2(0,0);
	mpos=vec2(400,300);
	glfwSetCursorPos(this->m_window, 800/2, 600/2);
	//cam->Rotate(modelOrientation);
}

quat MainGame::RotateQuat(const vec3 &rotation)
{
	m_VA = rotation.y+m_VA;
	m_HA = rotation.x+m_HA;
	if(m_VA>90)
		m_VA=90;
	if(m_VA<-90)
		m_VA = -90;
	if(m_HA<0)
		m_HA=360+m_HA;
	if(m_HA>=360)
		m_HA-=360;
	//std::cout << m_HA<<" HA " <<std::endl;
	return quat(vec3(radians(m_VA), radians(m_HA), 0));
}

GLvoid MainGame::render(GLdouble time)
{
	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->Use();
	glm::mat4 view;
	glm::mat4 projection;
	mat4 rot = glm::mat4_cast(modelOrientation);
	view = cam->GetViewMatrix();
	//projection = glm::perspective(45.0f, (GLfloat)this->WIDTH() / (GLfloat)this->HEIGHT(), 0.1f, 100.0f);
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	
	
	projection = cam->getProjectionMatrix();//glm::perspective(45.0f, (GLfloat)this->WIDTH() / (GLfloat)this->HEIGHT(), 0.1f, 100.0f);
	
	/*shader->setUniform("view", view, false);
	shader->setUniform("projection", projection, false);*/
	glBindVertexArray(VAO);

	
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
	//model *= rot;
	//model = glm::rotate(model, 20.0f, glm::vec3(1.0f, 0.3f, 0.5f));

	//left eye
	glEnable(GL_MULTISAMPLE);
	glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
	glViewport(0,0, renderwidth, renderheight);
	glClearColor( 0.15f, 0.15f, 0.18f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	auto lproj = convert(vrsys->GetProjectionMatrix(vr::EVREye::Eye_Left, cam->m_znear, cam->m_zfar, vr::EGraphicsAPIConvention::API_OpenGL));
	auto lview = view*inverse(mat4(convert(vrsys->GetEyeToHeadTransform(vr::EVREye::Eye_Left))));
	shader->setUniform("view", lview, false);
	shader->setUniform("projection", lproj, false);

	model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader->setUniform("model", model, false);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glm::mat4 model2;
	model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
	shader->setUniform("model", model2, false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->setUniform("model", glm::translate(mat4(), glm::vec3(-2.0f, -1.0f, 1.0f)), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->setUniform("model", glm::translate(mat4(), glm::vec3(0.0f, 0.0f, 15.0f)), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->setUniform("model", glm::translate(mat4(), glm::vec3(+2.0f, +1.0f, 15.0f)), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->setUniform("model", glm::translate(mat4(), glm::vec3(-2.0f, -1.0f, 15.0f)), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//multisample stuff
	glDisable(GL_MULTISAMPLE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, leftEyeDesc.m_nResolveFramebufferId);
	glBlitFramebuffer( 0, 0, renderwidth, renderheight, 0, 0, renderwidth, renderheight, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);	

	

	//right eye
	glEnable(GL_MULTISAMPLE);
	glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
	glViewport(0,0, renderwidth, renderheight);
	glClearColor( 0.15f, 0.15f, 0.18f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	auto rproj = convert(vrsys->GetProjectionMatrix(vr::EVREye::Eye_Right, cam->m_znear, cam->m_zfar, vr::EGraphicsAPIConvention::API_OpenGL));
	auto rview = view*inverse(mat4(convert(vrsys->GetEyeToHeadTransform(vr::EVREye::Eye_Right))));
	shader->setUniform("view", rview, false);
	shader->setUniform("projection", rproj, false);

	model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
	shader->setUniform("model", model, false);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, 0.0f));
	shader->setUniform("model", model2, false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->setUniform("model", glm::translate(mat4(), glm::vec3(-2.0f, -1.0f, 1.0f)), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->setUniform("model", glm::translate(mat4(), glm::vec3(0.0f, 0.0f, 15.0f)), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->setUniform("model", glm::translate(mat4(), glm::vec3(+2.0f, +1.0f, 15.0f)), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader->setUniform("model", glm::translate(mat4(), glm::vec3(-2.0f, -1.0f, 15.0f)), false);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//multisample stuff
	glDisable(GL_MULTISAMPLE);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEyeDesc.m_nResolveFramebufferId);
	glBlitFramebuffer( 0, 0, renderwidth, renderheight, 0, 0, renderwidth, renderheight, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);	


	//get right and left eye textures from framebuffer and submit those to the compositor for display on the HMD
	vr::Texture_t leftEyeTexture = {(void*)leftEyeDesc.m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture );
	vr::Texture_t rightEyeTexture = {(void*)rightEyeDesc.m_nResolveTextureId, vr::API_OpenGL, vr::ColorSpace_Gamma };
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture );
	glBindVertexArray(0);

	screenshader->Use();

	glViewport(0,0,WIDTH(), HEIGHT());
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT);
   
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, rightEyeDesc.m_nResolveTextureId);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0); 
	glfwSwapBuffers(this->m_window);
}

bool MainGame::CreateFrameBuffer( int nWidth, int nHeight, FramebufferDesc &framebufferDesc )
{
	//create multisample framebuffer
	glGenFramebuffers(1, &framebufferDesc.m_nRenderFramebufferId );
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nRenderFramebufferId);

	glGenRenderbuffers(1, &framebufferDesc.m_nDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, nWidth, nHeight );
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,	framebufferDesc.m_nDepthBufferId );

	glGenTextures(1, &framebufferDesc.m_nRenderTextureId );
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId );
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, nWidth, nHeight, true);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId, 0);

	//create resolve framebuffer as intermediate to the multisample framebuffer
	glGenFramebuffers(1, &framebufferDesc.m_nResolveFramebufferId );
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nResolveFramebufferId);

	glGenTextures(1, &framebufferDesc.m_nResolveTextureId );
	glBindTexture(GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId );
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

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	return true;
}
