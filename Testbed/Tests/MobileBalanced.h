/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef MOBILE_BALANCED_H
#define MOBILE_BALANCED_H

class MobileBalanced : public Test
{
public:

	enum
	{
		e_depth = 4
	};

	MobileBalanced()
	{
		b2Body* ground;

		// Create ground body.
		{
			b2BodyDef bodyDef;
			bodyDef.position.Set(0.0, 20.0);
			ground = this->m_world->CreateBody(&bodyDef);
		}

		float32 a = 0.5;
		b2Vec2 h(0.0, a);

		b2Body* root = AddNode(ground, b2Vec2(0, 0), 0, 3.0, a);

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = ground;
		jointDef.bodyB = root;
		jointDef.localAnchorA.SetZero();
		jointDef.localAnchorB.Assign(h);
		this->m_world->CreateJoint(&jointDef);
	}

	b2Body* AddNode(b2Body* parent, const b2Vec2& localAnchor, int32 depth, float32 offset, float32 a)
	{
		float32 density = 20.0;
		b2Vec2 h(0.0, a);

		b2Vec2 p = b2Vec2::Add(parent->GetPosition(), b2Vec2::Subtract(localAnchor, h));

		b2BodyDef bodyDef;
		bodyDef.type = b2Body::b2_dynamicBody;
		bodyDef.position.Assign(p);
		b2Body* body = this->m_world->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(0.25f * a, a);
		body->CreateFixture(&shape, density);

		if (depth == e_depth)
		{
			return body;
		}

		shape.SetAsBox(offset, 0.25f * a, b2Vec2(0, -a), 0.0);
		body->CreateFixture(&shape, density);

		b2Vec2 a1 = b2Vec2(offset, -a);
		b2Vec2 a2 = b2Vec2(-offset, -a);
		b2Body* body1 = AddNode(body, a1, depth + 1, 0.5 * offset, a);
		b2Body* body2 = AddNode(body, a2, depth + 1, 0.5 * offset, a);

		b2RevoluteJointDef jointDef;
		jointDef.bodyA = body;
		jointDef.localAnchorB.Assign(h);

		jointDef.localAnchorA.Assign(a1);
		jointDef.bodyB = body1;
		this->m_world->CreateJoint(&jointDef);

		jointDef.localAnchorA.Assign(a2);
		jointDef.bodyB = body2;
		this->m_world->CreateJoint(&jointDef);

		return body;
	}

	static Test* Create()
	{
		return new MobileBalanced;
	}
};

#endif
