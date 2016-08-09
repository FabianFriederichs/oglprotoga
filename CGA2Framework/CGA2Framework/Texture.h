#pragma once
#include "headers.h"
#include "Image2D.h"
#include "IDProvider.h"
#include "DDSLoader.h"
#include "glerror.h"

typedef enum {
	TEX_2D = 0,
	TEX_3D = 1,		//equals volume texture
	TEX_ARRAY = 2,
	TEX_CUBEMAP = 3,
	TEX_DX10_CUBEMAP = 4
} TEXTYPE;

class Texture
{
	friend class DDSLoader;
public:	
	Texture();
	Texture(const Texture& _other); //
	Texture(const std::string& _filepath); //single file texture
	Texture(const std::string& _filepath, const std::string& _name); //single file texture
	Texture(GLsizei _width, GLsizei _height);
	Texture(GLsizei _width, GLsizei _height, const std::string& _name);
	virtual ~Texture();

	bool loadData();
	bool unloadData();

	bool prepareAsRenderTarget(GLsizei _width, GLsizei _height, GLenum _internalformat, GLenum _format);

	//load / unload texture to ogl
	bool loadGLTexture(GLenum _wrapmodes, GLenum _wrapmodet, GLenum _minfilter, GLenum _magfilter);
	bool bindToTextureUnit(GLuint _unit);
	bool unbindFromTextureUnit(GLuint _unit);
	bool bind();
	bool unbind();
	bool freeGLTexture();

	//getters / setters
	const GLint getID() const { return m_id; }
	const GLuint getGLTexture() const { return m_texture; }

	const TEXTYPE getType() const { return m_type; }
	const GLuint getSizeX() const { return m_sizex; }
	const GLuint getSizeY() const { return m_sizey; }

	void setSize(GLuint _x, GLuint _y) { this->m_sizex = _x; this->m_sizey = _y; }
	void setType(TEXTYPE _type) { this->m_type = _type; }
		
	std::vector<std::vector<Image2D>>& getDataRef() { return m_data; }

	const bool isLoaded() { return m_isloaded; }
	const bool isBuffered() { return m_isbuffered; }
	const bool isBound() { return m_isbound; }
	const bool isRenderTarget() { return m_isrendertarget; }

	//surfaces and mipmaps
	void addSurface(const std::vector<Image2D>& _mipmappedimg) { m_data.push_back(_mipmappedimg); }
	void addMipMap(const int _surfaceindex, const Image2D& _image);

	const int getFaceCount() const { return m_faces.size(); }
	const int getMipMapCount(int _surfaceindex) const { try { return m_faces[_surfaceindex]; } catch (std::exception) { return -1; } }

private:
	GLint m_id;
	
	GLuint m_texture;
	std::string m_filepath;
	std::string m_name;
	//size of level0 texture
	GLuint m_sizex;
	GLuint m_sizey;

	//This is just for caching data from external texture files
	std::vector<std::vector<Image2D>> m_data; //faces->mipmaps of face->image data in Image2D

	//count faces and mipmaps
	std::vector<GLint> m_faces; //each face has GLint mipmaps

	//Type of the texture
	TEXTYPE m_type;

	bool m_isloaded;
	bool m_isbuffered;
	bool m_isbound;
	bool m_isrendertarget;

	//these formats are used when creating and bindung textures for offscreen rendering
	GLint m_internalformat;
	GLenum m_format;
	GLenum m_gltype;
};
