#pragma once
#include <algorithm> 
#include "Object.h"

class GameObject: public Object
{
	friend extern class Environment;

	public:
		GameObject(bool _solid, const char* _filename, int _numFrames = 1);
		virtual ~GameObject();

		void Update(float _elapsedTime) = 0;
		void Render(RenderWindow* _window) = 0;

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
				//if(kmap) delete kmap;
				//if(mmap) delete mmap;
			}

			Keyboard::Key* kmap;
			Mouse::Button* mmap;
		};

		void 	setSpeed(float _tankSpeed) { speed = _tankSpeed; }
		float 	getSpeed() const { return speed; }
		void 	setAngleSpeed(float _tankAngleSpeed) { speed = _tankAngleSpeed; }
		float 	getAngleSpeed() const { return speed; }
		void    setInput(Inputmap _imap) { imap = _imap; }

	protected:
		float speed, angleSpeed;
		Inputmap imap;

		void UpdateUserInput() = 0;

		// physics
		// the 2 suffix means squared
		float getDist2(GameObject* _obj) const;
		bool isInRadius(GameObject* _obj, float _radius2) const;
		bool isSolid() const;
		bool isCollidingWith(GameObject* _obj) const;
		void collisionFeedback(GameObject* _obj);
		//Vector2f getVelocityVector();

	private:
		bool solid;
};

