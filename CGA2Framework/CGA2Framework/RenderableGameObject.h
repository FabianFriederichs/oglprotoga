#pragma once
#include "GameObject.h"
#include "Model.h"
class RenderableGameObject :
	public GameObject
{
public:
	RenderableGameObject();
	virtual ~RenderableGameObject();

	//getters / setters
	Model* getModel() { return m_model; }
	void setModel(Model* _model) { m_model = _model; }

protected:
	Model* m_model;
};

