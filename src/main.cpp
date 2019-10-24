#include "Scene.h"

#include "CameraPerspective.h"

#include "PrimSphere.h"
#include "PrimPlane.h"
#include "PrimTriangle.h"

#include "ShaderFlat.h"
#include "ShaderEyelight.h"
#include "ShaderPhong.h"

#include "LightPoint.h"
#include "LightArea.h"
#include "timer.h"

Mat RenderFrame(void)
{
	// Define a scene
	CScene scene;
	
	// Load scene description
	scene.ParseOBJ("data/cow.obj");

#ifdef ENABLE_BSP
	// Build BSPTree
	scene.BuildAccelStructure();
#endif
	
	Vec3f pointLightIntensity(3, 3, 3);
	Vec3f lightPosition2(-3, 5, 4);
	Vec3f lightPosition3(0, 1, 4);
	
	scene.Add(std::make_shared<CLightPoint>(pointLightIntensity, lightPosition2));
	scene.Add(std::make_shared<CLightPoint>(pointLightIntensity, lightPosition3));

	Mat img(scene.m_pCamera->getResolution(), CV_32FC3);		// image array
	Ray ray;                                          			// primary ray

	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++) {
			scene.m_pCamera->InitRay(x, y, ray); // initialize ray
			img.at<Vec3f>(y, x) = scene.RayTrace(ray); 
		}
	
	img.convertTo(img, CV_8UC3, 255);
	return img;
}

int main(int argc, char* argv[])
{
	DirectGraphicalModels::Timer::start("Rebdering frame... ");
	Mat img = RenderFrame();
	DirectGraphicalModels::Timer::stop();
	imshow("Image", img);
	waitKey();
	imwrite("cow.jpg", img);
	return 0;
}
