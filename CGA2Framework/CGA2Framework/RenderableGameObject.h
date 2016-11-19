#pragma once
#include "GameObject.h"
#include "Model.h"
#include "glerror.h"

typedef enum{
	OPAQUE,
	TRANSPARENT,
	IGNORE
} GOTYPE;

class RenderableGameObject :
	public GameObject
{
public:
	RenderableGameObject(GOTYPE _type = OPAQUE);
	virtual ~RenderableGameObject();

	//getters / setters
	Model* getModel() { return m_model; }
	void setModel(Model* _model) { m_model = _model; }

	GOTYPE getType() { return m_type; }

protected:
	Model* m_model;
	GOTYPE m_type;
};

