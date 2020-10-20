#pragma once
#ifndef __BOX__
#define __BOX__

#include "DisplayObject.h"

class Box final : public DisplayObject {
public:
	Box();
	~Box();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	float GetAngle()
	{
		return m_angle;
	}
	void SetAngle(float angle)
	{
		m_angle = angle;
	}

private:
	void m_move();
	void m_checkBounds();
	void m_reset();

	float m_angle;
};


#endif /* defined (__BOX__) */