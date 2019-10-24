// Plane Geaometrical Primitive class
// Written by Sergey Kosov in 2005 for Rendering Competition
#pragma once

#include "Prim.h"

/**
 * @brief The Plane Geaometrical Primitive class
 */
class CPrimPlane : public CPrim
{
public:
	/**
	 * @brief Constructor
	 * @param origin Point on the plane
	 * @param normal Normal to the plane
	 */
	CPrimPlane(Vec3f origin, Vec3f normal, std::shared_ptr<IShader> pShader)
		: CPrim(pShader)
		, m_normal(normalize(normal))
		, m_origin(origin)
	{}
	virtual ~CPrimPlane(void) = default;

	virtual bool Intersect(Ray& ray) override
	{
		float t = (m_origin - ray.org).dot(m_normal) / ray.dir.dot(m_normal);
		if (t < Epsilon || t > ray.t) return false;
		ray.t = t;
		ray.hit = this;
		return true;
	}
	
	virtual Vec3f GetNormal(const Ray& ray) const override { return m_normal; }
	
	virtual CBoundingBox calcBounds(void) const override
	{
		CBoundingBox bounds;
		// --- PUT YOUR CODE HERE ---
		bounds.extend(m_origin);
		return bounds;
	}
	
private:
	Vec3f m_normal;	///< Point on the plane
	Vec3f m_origin;	///< Normal to the plane
};
