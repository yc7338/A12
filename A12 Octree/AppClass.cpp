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
			matrix4 m4Positions = glm::translate(glm::sphericalRand(10.0f));
			m_pMeshMngr->LoadModel("Minecraft\\Cube.obj", sInstance, false, m4Positions);
			m_pBOMngr->AddObject(sInstance);
	}

	m_pBOMngr->SetupOctree();

}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update(false);

	m_pBOMngr->Update(tree);

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();
	
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->PrintLine(std::to_string(nFPS), RERED);
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
	
	m_pMeshMngr->PrintLine("F5 toggle collision methods", RERED);
	m_pMeshMngr->PrintLine("F6 toggle Octree boxes", RERED);

	if (tree)
	{
		m_pMeshMngr->PrintLine("Octree", RERED);
	}
	else
	{
		m_pMeshMngr->PrintLine("Bruteforce", RERED);
	}
	if (SOBox)
		m_pBOMngr->DisplayTree();

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	m_pBOMngr->ReleaseInstance();
	super::Release(); //release the memory of the inherited fields
}