#pragma once

class Vec3f : public sf::Vector3f
{
public:
	float w = 0.0f;		//fourth vector value that is produced from matrix multiplication

public:
	Vec3f() : sf::Vector3f() {}
	//Vec3f(float x, float y, float z) : sf::Vector3f(x, y, z) {}
	Vec3f(float x, float y, float z, float _w = 0.0f) : sf::Vector3f(x, y, z), w(_w) {}
	~Vec3f() {}

	void show(void);			/* shows vector components to the console */
	void normalise(void);		/* overrides this vector by its normalised version */
	void divide_vec_by_W(void);	/* divides x,y,z components by 'W' value */
	float get_lenght(void);		/* returns vector lenght */

	Vec3f operator + (const Vec3f& rhs)	{ return Vec3f(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z); }
	Vec3f operator - (const Vec3f& rhs) { return Vec3f(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z); }
	Vec3f operator * (const float rhs)	{ return Vec3f(this->x * rhs, this->y * rhs, this->z * rhs); }
	Vec3f operator / (const float rhs)	{ return Vec3f(this->x / rhs, this->y / rhs, this->z / rhs,this->w); }
};
