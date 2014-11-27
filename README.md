Changelog
=========

(top is the newest)

* _26 november 2014_: Moving tank, no tilemap yet, no camera yet
* _26 november 2014_: Working entity component system

Code samples
============

Spawn an entity:

```C++
main_env.createEntity(
	Transform(500.f, 300.f, 0.f),
	Velocity(0.f, 0.f),
	TextureHandle("Tank_0.png"),
	TankControls(p1_keys)
);
```

Transform component:

```C++
struct Transform : public Component<Transform>
{
	Transform();
	Transform(float x, float y, float rot);

	float x, y;
	float rot;
};
```

Movement system:

```C++
void MovementSystem::update(Environment* env, float dt)
{
	Transform* transform = env->get<Transform>();
	Velocity* velocity= env->get<Velocity>();

	for(unsigned i=0;i<env->maxEntities();i++)
	{
		if(env->hasComponents<Transform, Velocity>(i))
		{
			transform[i].rot += velocity[i].vrot * dt;

			transform[i].x += cosf(transform[i].rot * PI/180.f + PI/2.f) * velocity[i].speed * dt;
			transform[i].y += sinf(transform[i].rot * PI/180.f + PI/2.f) * velocity[i].speed * dt;
		}
	}
}
```
