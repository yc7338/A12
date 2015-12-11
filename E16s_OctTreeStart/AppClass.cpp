#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("E16: Octree start"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(REAXISZ * 60.0f, REAXISY * 5.0f, REAXISY);

	m_pBOMngr = MyBOManager::GetInstance();

	m_nInstances = 1000;
	for (int i = 0; i < m_nInstances; i++)
	{
			String sInstance = "Cube_" + std::to_string(i);
			matrix4 m4Positions = glm::translate(glm::sphericalRand(20.0f));
			m_pMeshMngr->LoadModel("Minecraft\\Cube.obj", sInstance, false, m4Positions);
			m_pBOMngr->AddObject(sInstance);
	}


	//get all of the max and min gs

	std::vector<vector3> vertexList = std::vector<vector3>();

	for (int i = 0; i < m_nInstances; i++)
	{
		vertexList.push_back(m_pBOMngr->GetBoundingObject(i)->GetMaxG());
		vertexList.push_back(m_pBOMngr->GetBoundingObject(i)->GetMinG());
	}

	vector3 listMax = vertexList[0];
	vector3 listMin = vertexList[0];

	for (int i = 0; i < vertexList.size(); i++){
		if (vertexList[i].x > listMax.x)
			listMax.x = vertexList[i].x;
		else if (vertexList[i].x < listMin.x)
			listMin.x = vertexList[i].x;

		if (vertexList[i].y > listMax.y)
			listMax.y = vertexList[i].y;
		else if (vertexList[i].y < listMin.y)
			listMin.y = vertexList[i].y;

		if (vertexList[i].z > listMax.z)
			listMax.z = vertexList[i].z;
		else if (vertexList[i].z < listMin.z)
			listMin.z = vertexList[i].z;
	}

	//find center

	vector3 center = (listMax + listMin) / 2.0f;
	//find largest side of x, y, or z divide by 2

	float xLen = listMax.x - listMin.x;
	float yLen = listMax.y - listMin.y;
	float zLen = listMax.z - listMin.z;

	float maxSide = glm::max(xLen, yLen, zLen);
	m_pRoot = new MyOctant(center, maxSide / 2.0f);

	for (int i = 0; i < m_nInstances; i++)
	{
		m_pRoot->PushToRootObjectList(m_pBOMngr->GetBoundingObject(i));
	}

	m_pRoot->CheckSubdivision();
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update(false);

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();
	
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	m_pRoot->DisplayBox();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
		
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	m_pBOMngr->ReleaseInstance();
	super::Release(); //release the memory of the inherited fields
}