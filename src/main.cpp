#include "Renderer.h"
#include "Plane.h"
#include "Sphere.h"
#include "GeometryObj.h"
#include "Logger.h"
#include <iostream>
#include "Core.h"
#include <stdlib.h>
#include "helper/resource_helper.h"
#include "CudaLoader.h"

int main() {
	//TODO: Determine register limit for render_kernel to balance occupancy and register usage
	Logger::enabled = false;
	Logger::title = "DIV 32";

	//temporary hard-coded values to ensure we don't run out of memory
	//TODO: dynamically determine space needed
	size_t stackSize = 10000; //10kb stack size
	size_t heapSize = 4e7;
	cudaDeviceSetLimit(cudaLimitStackSize, stackSize);
	//cudaDeviceSetLimit(cudaLimitMallocHeapSize, heapSize);
	size_t sSize;
	size_t hSize;
	cudaDeviceGetLimit(&sSize, cudaLimitStackSize);
	cudaDeviceGetLimit(&hSize, cudaLimitMallocHeapSize);
	std::cout << "Stack Size: " << sSize << ", Heap Size: " << hSize << std::endl;

	//create image and set output path
	Image* image = new Image(1920, 1080);
    std::string outputImagePath = get_image_path("CUDA_test4.png");

    Camera* camera = new Camera();
	camera->setPosition(glm::vec3(0, 0, 1));

	//create scene and set background color or image
	Scene* scene = new Scene();
	scene->setBgColor(glm::vec3(0, 0, 0));


	//need specialized loading functionality if using cuda
	CudaLoader cudaLoader;	

	//cudaLoader needs to know number of Shape*'s that will be loaded to return pointers to the inside of the vector
		//transition to dynamically determined values once scene definition is loaded from file (Can then count shapes defined in file)
		//can also then dynamically determine heap and stack size needed on device
	cudaLoader.setNumShapesHint(Shape::TRIANGLE, 200);
	cudaLoader.setNumShapesHint(Shape::PLANE, 2);
	cudaLoader.setNumShapesHint(Shape::SPHERE, 1);
	cudaLoader.setNumShapesHint(Shape::TRIANGLE_MESH, 1);



	std::vector<std::unique_ptr<GeometryObj>> objectList;


	Plane** planeShape = cudaLoader.loadShape<Plane>(glm::vec3(0, -2, -2), 0.0f, 0.0f, 0.0f, glm::vec2(10,10));

	//create plane's material
	Material planeMat;
	planeMat.color = glm::vec3(128, 118, 115);
	planeMat.diffuseCoef = 0.8f;
	planeMat.type = Material::DIFFUSE;

	//create plane object that holds shape and material
	objectList.push_back(std::make_unique<GeometryObj>(planeShape, planeMat, "Plane"));

	Sphere** sphereShape = cudaLoader.loadShape<Sphere>(glm::vec3(-2.75, -1, -6), 1);
	Material sphereMat;
	sphereMat.color = glm::vec3(15, 175, 200);
	sphereMat.diffuseCoef = 0.8f;
	sphereMat.type = Material::DIFFUSE | Material::BPHONG_SPECULAR;
	sphereMat.specCoef = 0.2f;
	sphereMat.specularColor = glm::vec3(255, 255, 255);
	objectList.push_back(std::make_unique<GeometryObj>(sphereShape, sphereMat, "Sphere"));

	//create an area light to illuminate the scene
	//area light is a plane
	//area lights allow for soft shadows because
	//intensity of the shadow depends on area of light that is visible to the point
	glm::vec3 lightpos = glm::vec3(2, 6.0f, -6);
	Plane** lightPlane = cudaLoader.loadShape<Plane>(lightpos, degToRad(100.0f), degToRad(0.0f), degToRad(0.0f), glm::vec2(10.0f, 10.0f));
	std::unique_ptr<Light> light = std::make_unique<Light>();
	light->type = Light::POINT;
	//light->calcDirection(-45.0f, 0.0f, 0.0f);B
	light->pos = lightpos;
	light->color = glm::vec3(255, 197, 143);
	light->intensity = 10.0f;
	light->setShape(lightPlane);
	light->isAreaLight = true;
	scene->addLight(light.get());

	std::unique_ptr<Light> light_2 = std::make_unique<Light>();
	light_2->type = Light::POINT;
	light_2->color = glm::vec3(121, 169, 247);
	light_2->intensity = 3.0f;
	light_2->pos = glm::vec3(0.0f, 0.2f, 1.0f);
	scene->addLight(light_2.get());


	//create list of objects(meshes and materials) from .obj file, and add objects to the scene
	bool flipNormals = false;
	Logger::startClock("Load Time");
	//GeometryObj::loadOBJ(cudaLoader, get_obj_path("icosphere.obj"), &objectList, glm::vec3(3, -1, -6), flipNormals);
	GeometryObj::loadOBJ(cudaLoader, get_obj_path("box2.obj"), &objectList, glm::vec3(0, -.7, -6.2), flipNormals);
	Logger::record("Load Time");

	for (unsigned int i = 0; i < objectList.size(); ++i)
	{
		scene->addObject(objectList[i].get());
	}

	cudaLoader.loadShapePointers();
	light->finalize();
	cudaLoader.loadData(objectList);

	//create renderer with the initialized scene and image pointers
	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(scene, image, camera);
	//create core to handle assigning of rendering tasks
	Core core(renderer.get());

	//sets ambient lighting of the scene
		//minimum possible color of an unlit point
	scene->setAmbient(glm::vec3(255, 255, 255), 0.01f);

	//start logger, and then tell core to start rendering
	Logger::startClock("Render Time");
	core.render();
	Logger::record("Render Time");


	image->outputPNG(outputImagePath);
	Logger::printLog(get_log_path("Timing_Log_cuda.txt"));


	delete image;
	delete camera;
	delete scene;

	//To fix current issue in NVIDIA Visual Profiler
	CUDA_CHECK_ERROR(cudaDeviceSynchronize());
	CUDA_CHECK_ERROR(cudaProfilerStop());
	std::cout << "Done" << std::endl;

	return 0;
}



