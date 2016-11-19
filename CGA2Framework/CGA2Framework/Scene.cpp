#include "Scene.h"


Scene::Scene() :
m_camera(0),
m_directionallights(),
m_pointlights(),
m_spotlights(),
m_gameobjects(),
m_materials(),
m_models(),
//m_renderer(),
//m_renderList(),
m_textures(),
m_shaders(),
m_renderables()
{
	
}

Scene::Scene(GLint width, GLint height) :
m_camera(0),
m_directionallights(),
m_pointlights(),
m_spotlights(),
m_gameobjects(),
m_materials(),
m_models(),
//m_renderer(),
//m_renderList(),
m_textures(),
m_height(height),
m_width(width),
m_shaders(),
m_renderables()
{

}


Scene::~Scene()
{
	//clear();
}

//void Scene::render()
//{
//	//m_renderer.renderforward(m_renderList, &m_camera, m_directionallights, m_pointlights, m_spotlights);
//}



void Scene::save(std::string _path)
{
	std::ofstream file(_path.c_str());
	if (file.is_open())
	{
		file << "/resources\n";

		if (m_textures.size() > 0)
		{
			file << "\t/textures\n";
			int tc = 0;
			std::for_each(begin(m_textures), end(m_textures), [&](Texture2D* cur)
			{
				std::string p = cur->getPath();
				std::string n = cur->getName();
				if (!p.empty())
				{
					if (n.empty())
						cur->setName("tex" + std::to_string(tc++));
					file << "\t\t<" << cur->getName() << ">" << p << "\n";
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
					file << "\t\t<" << cur->getName() << "><" << std::to_string(tc) << '>' << p << "\n";
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
						file << "\t\t<" << std::to_string(tc++) << ">" << curt->getMaterial()->getName() << "\n";
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
				file << "\t<" << vec3ToString(cur->m_direction) << "><" << vec3ToString(s) << "><" << vec3ToString(r) << "><" << vec3ToString(tl) << "><" << vec4ToString(cur->m_lightcolor) << ">\n";
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
				file << "\t<" << vec4ToString(cur->m_lightcolor) << "><" << vec3ToString(s) << "><" << vec3ToString(r) << "><" << vec3ToString(tl) << "><" << vec3ToString(cur->getTransform().getForw()) << "><" << std::to_string(cur->m_constant) << "><" << std::to_string(cur->m_linear) << "><" << std::to_string(cur->m_quadratic) << "><" << std::to_string(cur->m_range) << ">\n";
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
}
void Scene::load(std::string _path)
{
	clear();
	std::string relPath = _path.substr(0,_path.find_last_of('\\')+1);
	std::ifstream file(_path.c_str());
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
	std::unordered_map<std::string, TextureCB*> cubemaps;
	std::unordered_map <std::string, Shader* > shaderss;
	std::unordered_map<std::string, Model*> modelss;
	std::unordered_map<std::string, Material*> materialss;
	std::unordered_map<std::string, RenderableGameObject*> gos;
	for (SNode n : nodes)
	{
		if (n.Value() == "/resources")
		{
			if (n.Contains("/textures"))
			{
				for (SNode* nt : n["/textures"].Children())
				{
					std::string v = nt->Value();
					int c = v.find_first_of('<') + 1;
					int cc = v.find_first_of('>');
					texturess[v.substr(c, cc - c)] = dynamic_cast<Texture2D*>(DDSLoader::loadDDSTex(relPath+v.substr(cc + 1)));
					addTexture(texturess[v.substr(c, cc - c)]);
				}
			}
			if (n.Contains("/cubemaps"))
			{
				for (SNode* nt : n["/cubemaps"].Children())
				{
					std::vector<std::string> vals;
					std::string v = nt->Value();
					vals.push_back(v.substr(0, v.find_first_of('<')));
					int c = 0, cc = 0;
					while ((c = v.find_first_of('<', cc)) != std::string::npos)
					{
						cc = v.find_first_of('>', ++c);
						vals.push_back(v.substr(c, cc - c));
					}

					if (vals.size() > 6)
					{
						TextureCB* cb = new TextureCB();
						cb->setName(vals[0]);

						for (Image2D i : texturess[vals[1]]->getData())
							cb->addMipMap(TextureCB::CBFACE_POSX, i);
						for (Image2D i : texturess[vals[2]]->getData())
							cb->addMipMap(TextureCB::CBFACE_NEGX, i);
						for (Image2D i : texturess[vals[3]]->getData())
							cb->addMipMap(TextureCB::CBFACE_POSY, i);
						for (Image2D i : texturess[vals[4]]->getData())
							cb->addMipMap(TextureCB::CBFACE_NEGY, i);
						for (Image2D i : texturess[vals[5]]->getData())
							cb->addMipMap(TextureCB::CBFACE_POSZ, i);
						for (Image2D i : texturess[vals[6]]->getData())
							cb->addMipMap(TextureCB::CBFACE_NEGZ, i);
						cb->setGLInternalFormat(texturess[vals[1]]->getGLInternalFormat());
						cb->setGLFormat(texturess[vals[1]]->getGLFormat());
						cb->setGLType(texturess[vals[1]]->getGLType());
						cb->setBindingOptions(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						cb->setWidth(texturess[vals[1]]->getWidth());
						cb->setHeight(texturess[vals[1]]->getHeight());
						if (cb != nullptr)
						{
							cubemaps[vals[0]] = cb;
							addCubeMap(cb);
						}
					}
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
					c = v.find_first_of('<', cc) + 1;
					cc = v.find_first_of('>', c);
					std::string vs = v.substr(c, cc - c);
					c = v.find_first_of('<', cc) + 1;
					cc = v.find_first_of('>', c);
					std::string fs = v.substr(c, cc - c);
					ForwardShader* forward = new ForwardShader();
					forward->load((relPath + vs).c_str(), (relPath + fs).c_str());
					shaderss[name] = forward;
					addShader(shaderss[name]);
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
					std::string p = relPath + v.substr(cc + 1);
					if (modelsloaded.find(p) == std::string::npos)
					{
						ms = OBJLoader::loadOBJ(p);
						for (Model* m : ms)
							addModel(m);
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
			for (SNode* nt : n.Children())
			{
				if (nt->Count() > 1 && nt->Contains("/textures"))
				{
					Material* m = new Material();
					if (nt->getNode(0).Value() != "/textures")
						m->setShader(shaderss[nt->getNode(0).Value()]);
					else
						m->setShader(shaderss[nt->getNode(1).Value()]);
					for (SNode* ntt : nt->getNode("/textures").Children())
					{
						m->addTexture(texturess[ntt->Value()]);
					}
					materialss[nt->Value()] = m;
					addMaterial(m);

				}
			}
		}
		else if (n.Value() == "/go")
		{
			for (SNode* nt : n.Children())
			{
				std::vector<std::string> vals;
				std::string v = nt->Value();
				vals.push_back(v.substr(0, v.find_first_of('<')));
				int c = 0, cc = 0;
				while ((c = v.find_first_of('<', cc)) != std::string::npos)
				{
					cc = v.find_first_of('>', ++c);
					vals.push_back(v.substr(c, cc - c));
				}

				if (vals.size() > 3)
				{
					RenderableGameObject* go = new RenderableGameObject();
					go->setName(vals[0]);
					if (vals[1] == "quad")
						go->setModel(new Model(PRIMITIVETYPE::QUAD));
					else
						go->setModel(modelss[vals[1]]);
					go->getTransform().setScale(vec3FromString(vals[2]));
					auto rot = vec3FromString(vals[3]);
					rot = vec3(radians(rot.x), radians(rot.y), radians(rot.z));
					go->getTransform().setRotate(rot);
					go->getTransform().setTranslate(vec3FromString(vals[4]));
					if (gos.count(vals[0])==0)
						gos[vals[0]] = go;
					addRenderable(go);
				}
			}
		}
		else if (n.Value() == "/submesh-material")
		{
			for (SNode* nt : n.Children())
			{
				auto meshes = gos[nt->Value()]->getModel()->getMeshes();
				for (SNode* ntt : nt->Children())
				{
					std::string v = ntt->Value();
					int c = v.find_first_of('<') + 1;
					int cc = v.find_first_of('>');
					int idx = std::stoi(v.substr(c, cc - c));

					std::string matname = v.substr(cc + 1);
					meshes[idx]->setMaterial(materialss[matname]);
				}
			}
		}
		else if (n.Value() == "/dirlights")
		{
			for (SNode* nt : n.Children())
			{
				std::vector<std::string> vals;
				std::string v = nt->Value();
				int c = 0, cc = 0;
				while ((c = v.find_first_of('<', cc)) != std::string::npos)
				{
					cc = v.find_first_of('>', ++c);
					vals.push_back(v.substr(c, cc - c));
				}
				if (vals.size() > 4)
				{
					DirectionalLight* dirlight = new DirectionalLight(vec3FromString(vals[0]), Transform(vec3FromString(vals[1]), vec3FromString(vals[2]), vec3FromString(vals[3])), vec4FromString(vals[4]));
					addDirectionalLight(dirlight);
				}
			}
		}
		else if (n.Value() == "/plights")
		{
			for (SNode* nt : n.Children())
			{
				std::vector<std::string> vals;
				std::string v = nt->Value();
				int c = 0, cc = 0;
				while ((c = v.find_first_of('<', cc)) != std::string::npos)
				{
					cc = v.find_first_of('>', ++c);
					vals.push_back(v.substr(c, cc - c));
				}
				if (vals.size() > 7)
				{
					PointLight* pointlight = new PointLight(vec4FromString(vals[0]), Transform(vec3FromString(vals[1]), vec3FromString(vals[2]), vec3FromString(vals[3])), std::stod(vals[4]), std::stod(vals[5]), std::stod(vals[6]), std::stod(vals[7]));
					addPointLight(pointlight);
				}
			}
		}
		else if (n.Value() == "/slights")
		{
			for (SNode* nt : n.Children())
			{
				std::vector<std::string> vals;
				std::string v = nt->Value();
				int c = 0, cc = 0;
				while ((c = v.find_first_of('<', cc)) != std::string::npos)
				{
					cc = v.find_first_of('>', ++c);
					vals.push_back(v.substr(c, cc - c));
				}
				if (vals.size() > 8)
				{
					SpotLight* spotlight = new SpotLight(vec4FromString(vals[0]), Transform(vec3FromString(vals[1]), vec3FromString(vals[2]), vec3FromString(vals[3])), vec3FromString(vals[4]), std::stod(vals[5]), std::stod(vals[6]), std::stod(vals[7]), std::stod(vals[8]));
					addSpotLight(spotlight);
				}
			}
		}
		else if (n.Value() == "/camera")
		{
			std::vector<std::string> vals;
			std::string v = n[1].Value();
			int c = 0, cc = 0;
			while ((c = v.find_first_of('<', cc)) != std::string::npos)
			{
				cc = v.find_first_of('>', ++c);
				vals.push_back(v.substr(c, cc - c));
			}
			if (vals.size() > 7)
			{
				//\t<fov><width><height><znear><zfar><wup><wright><wforw>
				FPSCamera* cam = new FPSCamera(std::stof(vals[0]), std::stoi(vals[1]), std::stoi(vals[2]), std::stof(vals[3]), std::stof(vals[4]), vec3FromString(vals[5]), vec3FromString(vals[6]), vec3FromString(vals[7]));
				cam->Fly(n[2].Value() == "1");
				cam->recalcProj();
				m_camera = cam;
			}
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
	nodes.clear();
}



void Scene::addRenderable(RenderableGameObject* _renderable)
{
	m_gameobjects.push_back(_renderable);

	GOTYPE t = _renderable->getType();
	std::pair<GOTYPE, RenderableGameObject*> p(t, _renderable);
	m_renderables.insert(p);

	/*for (std::vector<Mesh*>::iterator mit = _renderable->getModel()->getMeshes().begin(); mit != _renderable->getModel()->getMeshes().end(); mit = mit++)
	{
		RenderList::iterator insertpos = m_renderList.begin();
		while (insertpos != m_renderList.end() && (*mit)->getMaterial()->getID() < insertpos->first->getMaterial()->getID())
		{
			insertpos++;
		}
		m_renderList.insert(insertpos, RenderPair(*mit, _renderable));
	}*/
}

void Scene::removeRenderable(GLint _id)
{
	/*auto it = std::find_if(std::begin(m_gameobjects),
		std::end(m_gameobjects),
		[&](const RenderableGameObject v){ return v.getID() == _id; });*/
	/*m_renderList.erase(std::remove_if(m_renderList.begin(), m_renderList.end(), [_id](const RenderPair& x)
	{
		if (x.second->getID() == _id)
		{
			return true;
		}
		return false;
	}), m_renderList.end());*/

	m_gameobjects.erase(std::remove_if(m_gameobjects.begin(), m_gameobjects.end(), [_id](const RenderableGameObject* v)
	{ 
		if (v->getID() == _id)
		{
			if (v != nullptr)
				delete v;
			return true;
		}
		return false;
	}), m_gameobjects.end());
}

void Scene::addShader(Shader* _shader)
{
	m_shaders.push_back(_shader);
}

void Scene::addModel(Model* _model)
{
	m_models.push_back(_model);
}

void Scene::addMaterial(Material* _material)
{
	m_materials.push_back(_material);
}

void Scene::addTexture(Texture2D* _texture)
{
	m_textures.push_back(_texture);
}

void Scene::addDirectionalLight(DirectionalLight* _dirlight)
{
	m_directionallights.push_back(_dirlight);
}

void Scene::addPointLight(PointLight* _pointlight)
{
	m_pointlights.push_back(_pointlight);
}

void Scene::addSpotLight(SpotLight* _spotLight)
{
	m_spotlights.push_back(_spotLight);
}

void Scene::addCubeMap(TextureCB* _texture)
{
	m_cubemaps.push_back(_texture);
}

void Scene::addBillboard(Billboard* _billboard)
{
	m_billboards.push_back(_billboard);
}

void Scene::clear()
{
	//clear all the object collections in scene
	for(GameObject* g : m_gameobjects)
	{
		if (g != nullptr)
		{
			delete g;
			g = nullptr;
		}
	}
	m_gameobjects.clear();

	/*for (auto p : m_renderables)
	{
		if (p.second != nullptr)
		{
			delete p.second;
			p.second = nullptr;
		}
	}*/
	m_renderables.clear();

	for(DirectionalLight* l : m_directionallights)
	{
		if (l != nullptr)
		{
			delete l;
			l = nullptr;
		}
	}
	m_directionallights.clear();

	for(PointLight* l : m_pointlights)
	{
		if (l != nullptr)
		{
			delete l;
			l = nullptr;
		}
	}
	m_pointlights.clear();

	for(SpotLight* l : m_spotlights)
	{
		if (l != nullptr)
		{
			delete l;
			l = nullptr;
		}
	}
	m_spotlights.clear();

	for(Model* m : m_models)
	{
		if (m != nullptr)
		{
			delete m;
			m = nullptr;
		}
	}
	m_models.clear();

	for(Material* m : m_materials)
	{
		if (m != nullptr)
		{
			delete m;
			m = nullptr;
		}
	}
	m_materials.clear();

	for(Texture2D* t : m_textures)
	{
		if (t != nullptr)
		{
			delete t;
			t = nullptr;
		}
	}
	m_textures.clear();

	for(Shader* s : m_shaders)
	{
		if (s != nullptr)
		{
			delete s;
			s = nullptr;
		}
	}
	m_shaders.clear();

	if (m_camera != nullptr)
	{
		delete m_camera;
		m_camera = nullptr;
	}

	for(TextureCB* c : m_cubemaps)
	{
		if (c != nullptr)
		{
			delete c;
			c = nullptr;
		}
	}
	m_cubemaps.clear();
}

//
//void Scene::createRenderList()
//{
//	m_renderList.clear();
//	for each(auto v in m_gameobjects)
//	{
//		for each(auto m in v->getModel()->getMeshes())
//		{
//			m_renderList.push_back(RenderPair(m, v));
//		}
//	}
//	m_renderList.sort([](const std::pair<Mesh*, RenderableGameObject*>& lhs, const std::pair<Mesh*, RenderableGameObject*>& rhs) {
//		return lhs.first->getMaterial()->getID() < rhs.first->getMaterial()->getID();
//	});
//}
