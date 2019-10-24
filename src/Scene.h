#pragma once

#include "ILight.h"
#include "Prim.h"
#include "CameraPerspective.h"
#ifdef ENABLE_BSP
#include "BSPTree.h"
#endif

/**
 * @brief Scene class
 */
class CScene
{
public:
	/**
	 * @brief Constructor
	 * @param camera The reference to the camera
	 * @todo Background may be image
	 */
	CScene(Vec3f bgColor = RGB(0,0,0))
		: m_pCamera(std::make_unique<CCameraPerspective>(Vec3f(0, 3.5f, -13), Vec3f(0, 0, 1), Vec3f(0, 1, 0), 60, Size(800, 600)))
		, m_bgColor(bgColor)
	{}
  	~CScene(void) = default;
  
	
	/**
	 * @brief Loads the primitives from an .obj file and adds them to the scene
	 * @param fileName The full path to the .obj file
	 */
	void ParseOBJ(const std::string& fileName);
	
	/**
	 * @brief Adds a new primitive to the scene
	 * @param prim Pointer to the primitive
	 */
	void Add(const std::shared_ptr<CPrim> pPrim)
	{
		m_vpPrims.push_back(pPrim);
	}
	/**
	 * @brief Adds a new light to the scene
	 * @param pLight Pointer to the light
	 */
	void Add(const std::shared_ptr<ILight> pLight)
	{
		m_vpLights.push_back(pLight);
	}
  
	/**
	 * @brief Checks intersection of ray \b ray with all contained objects
	 * @param ray The ray
	 * @retval true If ray \b ray intersects any object
	 * @retval false otherwise
	 */
	bool Intersect(Ray& ray) const
	{
#ifdef ENABLE_BSP
		return m_pBSPTree->Intersect(ray);
#else
		bool hit = false;
		for (auto pPrim : m_vpPrims)
		  hit |= pPrim->Intersect(ray);
		return hit;
#endif
	}

	/**
	 * @brief Find occluder
	 * @param ray The ray
	 */
	bool Occluded(Ray& ray)
	{
#ifdef ENABLE_BSP
		return m_pBSPTree->Intersect(ray);
#else
		for (auto pPrim : m_vpPrims)
			if (pPrim->Occluded(ray)) return true;
		return false;
#endif
	}

#ifdef ENABLE_BSP
	/**
	 * @brief Calculates and return the bounding box, containing the whole scene
	 * @return The bounding box, containing the whole scene
	 */
	CBoundingBox CalcBounds(void)
	{
		CBoundingBox res;
		// --- PUT YOUR CODE HERE ---
		res.m_min = std::numeric_limits<float>::infinity();
		res.m_max = -1 * std::numeric_limits<float>::infinity();
		for (auto pPrim : m_vpPrims){
			res = res.extend(pPrim->calcBounds());
		}
		return res;
	}
	/**
	 * @brief Build the BSP tree for the current scene
	 * @details This function takes into accound all the primitives in scene and builds the BSP tree with the root node in \b m_pBSPTree variable
	 */
	void BuildAccelStructure(void)
	{
		CBoundingBox box = CalcBounds();
		std::cout << "Bounds are : " << box.m_min << " " << box.m_max << std::endl;
		m_pBSPTree = std::make_unique<BSPTree>(box, m_vpPrims);
	}
#endif
	
	/**
	 * @brief Traces the given ray and shade it
	 * @param ray The ray
	 * @return The color of the shaded ray
	 */
	Vec3f RayTrace(Ray& ray) const
	{
		return Intersect(ray) ? ray.hit->getShader()->Shade(ray) : m_bgColor;
	}


public:
	std::unique_ptr<CCameraPerspective>		m_pCamera;
	std::vector<std::shared_ptr<ILight>>	m_vpLights;						///< lights
	
private:
	Vec3f									m_bgColor	= RGB(0, 0, 0);    	///< background color
	std::vector<std::shared_ptr<CPrim>> 	m_vpPrims;						///< primitives
#ifdef ENABLE_BSP
	std::unique_ptr<BSPTree>				m_pBSPTree	= nullptr;
#endif
};
