#pragma once
#include "headers.h"
#include "Image2D.h"
#include "IDProvider.h"
#include "DDSLoader.h"

typedef enum {
	TEX_2D = 0,
	TEX_3D = 1,		//equals volume texture
	TEX_ARRAY = 2,
	TEX_CUBEMAP = 3,
	TEX_DX10_CUBEMAP = 4
} TEXTYPE;

class Texture
{
public:
	Texture();
	Texture(const Texture& _other);
	Texture(const std::string& _filepath); //single texture
	Texture(const std::string& _filepath, const std::string& _name); //single texture
	Texture(const bool isRenderTarget, GLsizei _width, GLsizei _height);
	Texture(const bool isRenderTarget, GLsizei _width, GLsizei _height, const std::string& _name);
	~Texture();

	bool loadData();
	bool unloadData();

	bool prepareAsRenderTarget(GLsizei _width, GLsizei _height, GLenum _internalformat, GLenum _format);

	//load / unload texture to ogl
	bool loadGLTexture(GLenum _wrapmodes, GLenum _wrapmodet, GLenum _minfilter, GLenum _magfilter);
	bool bindToTextureUnit(GLuint _unit);
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

	//surfaces and mipmaps
	void addSurface(const std::vector<Image2D>& _mipmappedimg) { m_data.push_back(_mipmappedimg); }
	void addMipMap(const int _surfaceindex, const Image2D& _image);

	const int getSurfaceCount() const { return m_data.size(); }
	const int getMipMapCount(int _surfaceindex) const { try { return m_data[_surfaceindex].size(); } catch (std::exception) { return -1; } }

private:
	GLint m_id;
	
	GLuint m_texture;
	std::string m_filepath;
	std::string m_name;
	GLuint m_sizex;
	GLuint m_sizey;
	std::vector<std::vector<Image2D>> m_data; //faces->mipmaps of face->image data in Image2D

	TEXTYPE m_type;

	bool m_isloaded;
	bool m_isbuffered;
	bool m_isbound;
	bool m_isrendertarget;

	GLenum m_rtinternalformat;
	GLenum m_rtformat;
};
