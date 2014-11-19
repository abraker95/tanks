#pragma once
#include <algorithm> 
#include "VectorLN.h"
#include "Object.h"

// for debugging
#include "DEBUG.h"
#include "Line.h"

class GameObject: public Object
{
	friend class Environment;  /// \TODO: Move to the Object class
	friend class GameScene;

	public:
		GameObject(bool _solid, const char* _filename, int _numFrames = 1);
		virtual ~GameObject();

		void Update(float _elapsedTime) = 0;
		//void Render(RenderWindow* _window) = 0;

		struct Inputmap
		{
			Inputmap() {}
			Inputmap(Keyboard::Key* _kmap, Mouse::Button* _mmap)
			{ 
				kmap = _kmap;
				mmap = _mmap;
			}

			~Inputmap()
			{
				// if(kmap) delete kmap;
				// if(mmap) delete mmap;
			}

			Keyboard::Key* kmap;
			Mouse::Button* mmap;
		};

		void setInput(Inputmap _imap) { imap = _imap; }

		void addHealth(int _health) { health += _health; }
		bool hasHealth() const { return health > 0; }
		int  getHealth() const { return health; }
		void resetHealth() { health = maxHealth; }
		int getMaxHealth() const { return maxHealth; }

	protected:
		//float speed, angleSpeed;
		int health;
		int maxHealth;
		//Inputmap imap;

		//void UpdateUserInput() = 0;
		
		// physics
		// the 2 suffix means squared
		float getDist2(GameObject* _obj) const;
		bool isInRadius(GameObject* _obj, float _radius2) const;
		bool isSolid() const;
		bool isCollidingWith(GameObject* _obj) const;
		void collisionFeedback(GameObject* _obj);

		VectorLN getVelocityVector();
		VectorLN getSurfaceTangentVector();
		VectorLN getSurfaceNormalVector();
		float getNextAngle();
		bool isCollision(GameObject* _obj);
	
	private:
		// Note: Put these functions in a physics management class if we ever make one
		static bool intersectCircleRect(GameObject* _obj1, GameObject* _obj2);
		static bool intersectCircleCircle(GameObject* _obj1, GameObject* _obj2);
		static bool intersectCirclePoint(GameObject* _obj1, const Vector2f& _pt);
		static bool intersectCircleLine(GameObject* _obj1, const Vector2f& _l1, const Vector2f& _l2);
		static bool intersectRectRect(GameObject* _obj1, GameObject* _obj2);
		static bool intersectRectPoint(GameObject* _obj1, const Vector2f& _pt);

		static Vector2f responseCircleRect(GameObject* _obj1, GameObject* _obj2);
		static Vector2f responseCircleCircle(GameObject* _obj1, GameObject* _obj2);

	protected:
		float speed, angleSpeed;
		Inputmap imap;

		void UpdateUserInput() = 0;

	private:
		bool solid;
};
