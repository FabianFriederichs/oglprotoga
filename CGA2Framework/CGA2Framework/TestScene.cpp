#include "TestScene.h"

void split(const std::string &s, char delim, std::vector<std::string> &elems);


std::vector<std::string> split(const std::string &s, char delim);

const std::string vec3ToString(const glm::vec3& v);

const std::string vec4ToString(const glm::vec4& v);


const glm::vec3 vec3FromString(const std::string& v);

const glm::vec4 vec4FromString(const std::string& v);

const std::string Peek(std::ifstream &strim)
{
	std::string line;
	int len = strim.tellg();
	// Read line
	std::getline(strim, line);
	// Return to position before "Read line".
	strim.seekg(len, std::ios_base::beg);
	return line;
}



TestScene::TestScene(GLint width, GLint height) : Scene(width, height)
{
}


TestScene::~TestScene()
{
}

void TestScene::load(){}

void TestScene::load(const std::string &path)
{
	std::ifstream file(path.c_str());
	std::list<SNode> nodes;
	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			if (line[0] == '/')
			{
				nodes.push_back(SNode(line));
			}
			else if (line[0] == '\t')
			{
				SNode* curnode = &nodes.back();
				int c = 1;
				while (line[c] == '\t')
				{
					c++;
					if (curnode->Count() > 0)
						curnode = &curnode->getNode(-1);
					else
						goto skip;
				}
				curnode->AddChild(line.substr(c));
			}
		skip:;
		}
	}
	file.close();
	std::unordered_map<std::string, Texture2D*> texturess;
	std::unordered_map < std::string, Shader* > shaderss;
	std::unordered_map<std::string, Model*> modelss;
	std::unordered_map<std::string, Material*> materialss;
	for (SNode n : nodes)
	{
		if (n.Value() == "/resources")
		{
			if (n.Contains("/textures"))
			{
				for (SNode* nt : n["/textures"].Children())
				{
					std::string v = nt->Value();
					int c = v.find_first_of('<')+1;
					int cc = v.find_first_of('>');
					texturess[v.substr(c, cc - c)] = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex(v.substr(cc + 1)));
				}
			}
			if (n.Contains("/shaders"))
			{
				for (SNode* nt : n["/shaders"].Children())
				{
					std::string v = nt->Value();
					int c = v.find_first_of('<') + 1;
					int cc = v.find_first_of('>');
					std::string name = v.substr(c, cc - c);
					c = v.find_first_of('<',cc) + 1;
					cc = v.find_first_of('>', c);
					std::string vs = v.substr(c, cc - c);
					c = v.find_first_of('<', cc) + 1;
					cc = v.find_first_of('>', c);
					std::string fs = v.substr(c, cc - c);
					ForwardShader* forward = new ForwardShader();
					forward->load(vs.c_str(), fs.c_str());
					shaderss[name] = forward;
				}
			}
			if (n.Contains("/models"))
			{
				std::string modelsloaded;
				std::list<Model*> ms;
				for (SNode* nt : n["/models"].Children())
				{
					std::string v = nt->Value();
					int c = v.find_first_of('<') + 1;
					int cc = v.find_first_of('>');
					std::string name = v.substr(c, cc - c);
					c = v.find_first_of('<', cc) + 1;
					cc = v.find_first_of('>', c);
					int index = std::stoi(v.substr(c, cc - c));
					std::string p = v.substr(cc+1);
					if (modelsloaded.find(p) == std::string::npos)
					{
						ms = OBJLoader::loadOBJ(p);
						modelsloaded += p + "\t";
					}
					if (ms.size() > index)
					{
						auto it = std::begin(ms);
						std::advance(it, index);
						modelss[name] = *it;
					}
				}
			}
		}
		else if (n.Value() == "/materials")
		{
			for (SNode* nt : n["/materials"].Children())
			{
				Material* m = new Material();
				if (nt->Count() > 1 && nt->Contains("/textures"))
				{
					if (nt[0].Value() != "/textures")
						m->setShader(shaderss[nt[0].Value()]);
					else
						m->setShader(shaderss[nt[1].Value()]);
					for (SNode* ntt : nt->getNode("/textures").Children())
					{
						m->addTexture(texturess[ntt->Value()]);
					}
				}
			}
		}
		else if (n.Value() == "/submesh-material")
		{
			for (SNode* nt : n["/submesh-material"].Children())
			{
				auto meshes = modelss[nt->Value()]->getMeshes();
				for (SNode* ntt : nt->Children())
				{
					std::string v = nt->Value();
					int c = v.find_first_of('<') + 1;
					int cc = v.find_first_of('>');
					int idx = std::stoi(v.substr(c, cc - c));
					c = v.find_first_of('<', cc) + 1;
					cc = v.find_first_of('>', c);
					std::string matname = v.substr(c, cc - c);
					meshes[idx]->setMaterial(materialss[matname]);
				}

			}
		}
		else if (n.Value() == "/go")
		{

		}
		else if (n.Value() == "/dirlights")
		{

		}
		else if (n.Value() == "/plights")
		{

		}
		else if (n.Value() == "/slights")
		{

		}
		else if (n.Value() == "/camera")
		{

		}
		else if (n.Value() == "/width")
		{
			m_width = std::stoi(n[0].Value());
		}
		else if (n.Value() == "/height")
		{
			m_height = std::stoi(n[0].Value());
		}
	}
	/*

	file format:
	/resources
	\t/textures
	\t\t<name>path
	\t/shaders
	\t\t<name><Vpath><Fpath>
	\t/models
	\t\t<name>path

	/materials
	\tname
	\t\tshadername
	\t\t/textures
	\t\t\ttexturename
	...
	/submesh-material
	\t<modelname>
	\t\t<submeshid>materialname
	/go
	\tmodelname<scale><rotation><translation>
	/dirlights
	\t<dir><scale><rotate><translate><color>
	/plights
	\t<color><scale><rotate><translate><constant><lineear><quadratic><range>
	/slights
	\t<color><scale><rotate><translate><dir><constannt>linear><quadratic><range>
	/camera
	\ttype
	\t<fov><width><height><znear><zfar><wup><wright><wforw>
	\tfly(true/false)
	/width<>
	/height<>

	-textures
	-shaders (shader pairs)
	-materials
	-1:m textures
	-shader
	-models
	-submeshes <> material

	-renderable GO
	-model
	-transform

	-dirlights
	-glm::vec3 _direction, Transform _transform, glm::vec4 _color
	-plights
	-glm::vec4 _color, Transform _transform, GLfloat _constant, GLfloat _linear, GLfloat _quadratic, GLfloat _range
	-slights
	-glm::vec4 _color, Transform _transform, glm::vec3 _direction, GLfloat _constant, GLfloat _linear, GLfloat _quadratic, GLfloat _range
	-1camera
	-camera type
	-GLfloat fov,GLint width,GLint height,GLfloat znear,GLfloat zfar, vec3 worldup, vec3 worldright, vec3 worldforward
	-fly mode
	width
	height

	1. Load all resources and map them to their names specified in scene file
	2. create objects as defined in scene file by their associated data (resource names, initialization data)
	3.
	
	*/

	//load testmodel "uglypot"
	std::list<Model*> models = OBJLoader::loadOBJ("Assets\\Models\\uglypot.obj");
	Texture2D* wooddiff = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\wooddiff_RGBA32UI.dds"));
	Texture2D* woodspec = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\woodspec_RGBA32UI.dds"));
	Texture2D* woodgloss = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\woodgloss_RGBA32UI.dds"));
	Texture2D* woodnormal = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\woodnormal_RGBA32UI.dds"));
	Texture2D* woodheight = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\woodheight_RGBA32UI.dds"));
	Texture2D* metaldiff = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metaldiff_RGBA32UI.dds"));
	Texture2D* metalspec = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metalspec_RGBA32UI.dds"));
	Texture2D* metalgloss = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metalgloss_RGBA32UI.dds"));
	Texture2D* metalnormal = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metalnormal_RGBA32UI.dds"));
	Texture2D* metalheight = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex("Assets\\Materials\\metalheight_RGBA32UI.dds"));
	//this thing will be forward shaded
	ForwardShader* forward = new ForwardShader();
	forward->load("BaseVertex.vert", "BaseFrag.frag");
	addShader(forward);

	

	addTexture(wooddiff);
	addTexture(woodspec);
	addTexture(woodgloss);
	addTexture(woodnormal);
	addTexture(woodheight);
	addTexture(metaldiff);
	addTexture(metalspec);
	addTexture(metalgloss);
	addTexture(metalnormal);
	addTexture(metalheight);

	//load wood and metal material
	Material* wood = new Material();
	wood->addTexture(wooddiff);
	wood->addTexture(woodspec);
	wood->addTexture(woodgloss);
	wood->addTexture(woodnormal);
	wood->addTexture(woodheight);
	wood->setShader(forward);
	Material* metal = new Material();
	metal->addTexture(metaldiff);
	metal->addTexture(metalspec);
	metal->addTexture(metalgloss);
	metal->addTexture(metalnormal);
	metal->addTexture(metalheight);
	metal->setShader(forward);

	addMaterial(wood);
	addMaterial(metal);
	std::list<Model*>::iterator moit = models.begin();
	std::vector<Mesh*>::iterator mshit = (*moit)->getMeshes().begin();

	//2 submeshes
	(*mshit++)->setMaterial(wood);
	(*mshit)->setMaterial(metal);

	m_models.splice(end(m_models), models);
	//create an instance of this model and add it to the scene gameobjects
	RenderableGameObject* go = new RenderableGameObject();
	go->setModel(*moit);
	go->getTransform().setTranslate(glm::vec3(0.0f, 0.0f, -2.0f));
	go->setName("box");
	addRenderable(go);

	//create a directional light
	DirectionalLight* dirlight = new DirectionalLight(glm::vec3(1.0f, -1.0f, -1.0f), Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)), glm::vec4(1.0f, 1.0f, 0.8f, 1.0f));
	addDirectionalLight(dirlight);

	PointLight* pointlight = new PointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)),1.0f, 0.7f, 1.8f, 7.0f);
	addPointLight(pointlight);
	//create a flycam
	FPSCamera* cam = new FPSCamera(45.0f, 800, 600, 0.1f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	cam->Fly(true);
	cam->recalcProj();
	m_camera = cam;

	int dummy = 0;
}

void TestScene::save(const std::string& path)
{
	std::ofstream file(path.c_str());
	if (file.is_open())
	{
		file << "/resources\n";
		
		if (m_textures.size() > 0)
		{
			file <<"\t/textures\n";
			int tc = 0;
			std::for_each(begin(m_textures), end(m_textures), [&](Texture2D* cur)
			{
				std::string p = cur->getPath();
				std::string n = cur->getName();
				if (!p.empty())
				{
					if (n.empty())
						cur->setName("tex"+std::to_string(tc++));
					file << "\t\t<"<<cur->getName()<<">"<<p<<"\n";
				}
			});
		}
		

		if (m_shaders.size() > 0)
		{
			file << "\t/shaders\n";
			std::for_each(begin(m_shaders), end(m_shaders), [&](Shader* cur)
			{
				std::string v = cur->getVPath();
				std::string f = cur->getFPath();
				if (!v.empty() && !f.empty())
				{
					file << "\t\t<" << cur->getID() << ">" << "<" << v << ">" << "<" << f << ">" << "\n";
				}
			});
		}
		

		if (m_models.size() > 0)
		{
			file << "\t/models\n";
			int tc = 0;
			std::string op;
			std::for_each(begin(m_models), end(m_models), [&](Model* cur)
			{
				std::string p = cur->getPath();
				std::string n = cur->getName();
				
				if (!p.empty())
				{
					if (op != p)
					{
						op = p;
						tc = 0;
					}
					if (n.empty())
						cur->setName("model" + std::to_string(tc));
					file << "\t\t<" << cur->getName() << "><"<< std::to_string(tc)<< '>'<<p<< "\n";
					tc++;
				}
			});
		}
		

		if (m_materials.size() > 0)
		{
			file << "/materials\n";
			int tc = 0;
			std::for_each(begin(m_materials), end(m_materials), [&](Material* cur)
			{
				std::string n = cur->getName();
				if (n.empty())
					cur->setName("material" + std::to_string(tc++));

				file << "\t" << cur->getName() << "\n";
				file << "\t\t" << cur->getShader()->getID() << "\n";
				file << "\t\t/textures" << "\n";
				std::for_each(begin(cur->getTextures()), end(cur->getTextures()), [&](Texture* curt)
				{
					file << "\t\t\t" << curt->getName() << "\n";
				});
			});
		}		


		if (m_models.size() > 0)
		{
			file << "/submesh-material\n";
			
			std::for_each(begin(m_models), end(m_models), [&](Model* cur)
			{
				if (cur->getMeshes().size() > 0)
				{
					file << "\t" << cur->getName() << "\n";
					int tc = 0;
					std::for_each(begin(cur->getMeshes()), end(cur->getMeshes()), [&](Mesh* curt)
					{
						file << "\t\t<" << std::to_string(tc++)<< ">"<<curt->getMaterial()->getName()<<"\n";
					});
				}
			});
		}		

		if (m_gameobjects.size() > 0)
		{
			file << "/go\n";

			std::for_each(begin(m_gameobjects), end(m_gameobjects), [&](RenderableGameObject* cur)
			{
				auto t = cur->getTransform();
				auto r = t.getRotate();
				auto s = t.getScale();
				auto tl = t.getTranslate();
				file << "\t" << cur->getModel()->getName() << "<" << vec3ToString(s) << "><" << vec3ToString(r) << "><" << vec3ToString(tl) << ">\n";
			});
		}		

		if (m_directionallights.size() > 0)
		{
			file << "/dirlights\n";

			std::for_each(begin(m_directionallights), end(m_directionallights), [&](DirectionalLight* cur)
			{
				auto t = cur->getTransform();
				auto r = t.getRotate();
				auto s = t.getScale();
				auto tl = t.getTranslate();
				file << "\t<"<< vec3ToString(cur->m_direction) << "><" << vec3ToString(s) << "><" << vec3ToString(r) << "><" << vec3ToString(tl) <<"><"<<vec4ToString(cur->m_lightcolor)<< ">\n";
			});
		}		

		if (m_pointlights.size() > 0)
		{
			file << "/plights\n";

			std::for_each(begin(m_pointlights), end(m_pointlights), [&](PointLight* cur)
			{
				auto t = cur->getTransform();
				auto r = t.getRotate();
				auto s = t.getScale();
				auto tl = t.getTranslate();
				file << "\t<" << vec4ToString(cur->m_lightcolor) << "><" << vec3ToString(s) << "><" << vec3ToString(r) << "><" << vec3ToString(tl) << "><" << std::to_string(cur->m_constant) << "><" << std::to_string(cur->m_linear) << "><" << std::to_string(cur->m_quadratic) << "><" << std::to_string(cur->m_range) << ">\n";
			});
		}		

		if (m_spotlights.size() > 0)
		{
			file << "/slights\n";

			std::for_each(begin(m_spotlights), end(m_spotlights), [&](SpotLight* cur)
			{
				auto t = cur->getTransform();
				auto r = t.getRotate();
				auto s = t.getScale();
				auto tl = t.getTranslate();
				file << "\t<" << vec4ToString(cur->m_lightcolor) << "><" << vec3ToString(s) << "><" << vec3ToString(r) << "><" << vec3ToString(tl) << "><" << vec3ToString(cur->m_direction) << "><" << std::to_string(cur->m_constant) << "><" << std::to_string(cur->m_linear) << "><" << std::to_string(cur->m_quadratic) << "><" << std::to_string(cur->m_range) << ">\n";
			});
		}		

		if (m_camera != nullptr)
		{
			file << "/camera\n";
			if (dynamic_cast<FPSCamera*>(m_camera))
			{
				auto c = dynamic_cast<FPSCamera*>(m_camera);
				std::string n = typeid(FPSCamera).name();
				file << "\t" << n.substr(n.find_first_of(' ')) << "\n";
				file << "\t<" << std::to_string(c->getFov()) << "><" << std::to_string(c->getWidth()) << "><" << std::to_string(c->getHeight()) << "><" << std::to_string(c->getNear()) << "><" << std::to_string(c->getFar()) << "><" << vec3ToString(c->getWorldUp()) << "><" << vec3ToString(c->getWorldRight()) << "><" << vec3ToString(c->getWorldForw()) << ">\n";
				file << "\t" << std::to_string(c->isFlying()) << "\n";
			}
			else
			{
				file << "\t" << typeid(Camera).name() << "\n";
				file << "\t<" << std::to_string(m_camera->getFov()) << "><" << std::to_string(m_camera->getWidth()) << "><" << std::to_string(m_camera->getHeight()) << "><" << std::to_string(m_camera->getNear()) << "><" << std::to_string(m_camera->getFar()) << "><" << vec3ToString(m_camera->getWorldUp()) << "><" << vec3ToString(m_camera->getWorldRight()) << "><" << vec3ToString(m_camera->getWorldForw()) << ">\n";
			}
			
		}
		file << "/width\n\t" << std::to_string(m_width) << "\n";
		file << "/height\n\t" << std::to_string(m_height) << "\n";

		file.close();
	}


	/*
	file format:
	/resources
	\t/textures
	\t\t<name>path
	\t/shaders
	\t\t<name><Vpath><Fpath>
	\t/models
	\t\t<name>path

	/materials
	\tname
	\t\tshadername
	\t\t/textures
	\t\t\ttexturename
	...
	/submesh-material
	\t<modelname>
	\t\t<submeshid>materialname
	/go
	\tmodelname<scale><rotation><translation>
	/dirlights
	\t<dir><scale><rotate><translate><color>
	/plights
	\t<color><scale><rotate><translate><constant><lineear><quadratic><range>
	/slights
	\t<color><scale><rotate><translate><dir><constannt>linear><quadratic><range>
	/camera
	\ttype
	\t<fov><width><height><znear><zfar><wup><wright><wforw>
	\tfly(true/false)
	/width<>
	/height<>
	*/

}

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

const std::string vec3ToString(const glm::vec3& v)
{
	std::string s = "";
	s += std::to_string(v.x)+",";
	s += std::to_string(v.y) + ",";
	s += std::to_string(v.z);
	return s;
}

const std::string vec4ToString(const glm::vec4& v)
{
	std::string s = "";
	s += std::to_string(v.x) + ",";
	s += std::to_string(v.y) + ",";
	s += std::to_string(v.z) + ",";
	s += std::to_string(v.w);
	return s;
}


const glm::vec3 vec3FromString(const std::string& v)
{
	
	auto vec = split(v, ',');
	GLfloat x(std::stod(vec[0])), y(std::stod(vec[1])), z(std::stod(vec[2]));
	return glm::vec3(x,y,z);
}

const glm::vec4 vec4FromString(const std::string& v)
{
	auto vec = split(v, ',');
	GLfloat x(std::stod(vec[0])), y(std::stod(vec[1])), z(std::stod(vec[2])), w(std::stod(vec[3]));

	return glm::vec4(x, y, z, w);
}



