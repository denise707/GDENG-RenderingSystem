#include "Cube.h"
#include <iostream>
#include "DeviceContext.h"
#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "SceneCameraHandler.h"
#include "Utils.h"

//Utils* utils = new Utils();
Cube::Cube(string name, void* shaderByteCode, size_t sizeShader) :AGameObject(name)
{
	//Create buffers for drawing. Vertex data that needs to be drawn are temporarily placed here.
	Vertex vertex_list[] =
	{
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,1,1),  Vector3D(0,1,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,1), Vector3D(0,1,1) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,1),  Vector3D(1,0,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,1,1), Vector3D(0,0,1) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(1,1,1), Vector3D(0,1,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(1,1,1), Vector3D(0,1,1) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(1,1,1),  Vector3D(1,0,0) },
		{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(1,1,1), Vector3D(0,0,1) }
	};

	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->vertexBuffer->load(vertex_list, sizeof(Vertex), ARRAYSIZE(vertex_list), shaderByteCode, sizeShader);

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0, 1, 2, 
		2, 3, 0,
		//BACK SIDE
		4, 5, 6,
		6, 7, 4,
		//TOP SIDE
		1, 6, 5,
		5, 2, 1,
		//BOTTOM SIDE
		7, 0, 3,
		3, 4, 7,
		//RIGHT SIDE
		3, 2, 5,
		5, 4, 3,
		//LEFT SIDE
		7, 6, 1,
		1, 0, 7
	};

	this->indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	this->indexBuffer->load(index_list, ARRAYSIZE(index_list));

	//Create constant buffer
	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));

	setAnimSpeed(4);
}

Cube::~Cube()
{
	this->vertexBuffer->release();
	this->indexBuffer->release();
}

void Cube::update(float delta_time)
{
	//For animation
	/*float newRotX = getLocalRotation().m_x + (delta_time * speed);
	float newRotY = getLocalRotation().m_y + (delta_time * speed);
	float newRotZ = getLocalRotation().m_z + (delta_time * speed);

	setRotation(newRotX, newRotY, newRotZ);

	deltaTime += delta_time;*/
}

void Cube::draw(int width, int height, VertexShader* vertex_shader, PixelShader* pixel_shader)
{
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphEngine->getImmediateDeviceContext();

	CBData cbData = {};

	cbData.time = deltaTime;

	//Add object transformation
	Matrix4x4 temp;

	cbData.worldMatrix.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setScale(getLocalScale());
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationX(getLocalRotation().m_x);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(getLocalRotation().m_y);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(getLocalRotation().m_z);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setTranslation(getLocalPosition());
	cbData.worldMatrix *= temp;

	//Add camera transformation
	Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.viewMatrix = cameraMatrix;

	//Perspective View
	cbData.projMatrix.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	//Orthographic View
	//cbData.projMatrix.setOrthoLH(width / 300.0f, height / 300.0f,-4.0f,4.0f);

	this->constantBuffer->update(deviceContext, &cbData);
	deviceContext->setConstantBuffer(vertex_shader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixel_shader, this->constantBuffer);

	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setVertexBuffer(this->vertexBuffer);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void Cube::setAnimSpeed(float speed)
{
	this->speed = speed;
}


