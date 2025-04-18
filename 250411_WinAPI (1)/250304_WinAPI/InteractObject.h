#pragma once
#include "GameObject.h"



class InteractObject : public GameObject
{
	virtual void Interact();

	virtual void Pick();

	virtual void Picked();

	virtual void Move();

	InteractObject* owner;
	InteractObject* mine;


};

