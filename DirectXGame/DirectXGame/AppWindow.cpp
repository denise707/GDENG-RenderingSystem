#include "AppWindow.h"
#include <Windows.h>
#include "EngineTime.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "SceneCameraHandler.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

void AppWindow::update()
{
	/*constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;*/
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);
	EngineTime::initialize();

	//srand((unsigned int)time(NULL));

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);
	
	//Initialize Scene Camera
	SceneCameraHandler::initialize();

	shader_byte_code = nullptr;
	size_shader = 0;

	//Vertex Shader
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	//Create Object Instances
	Cube* cube = new Cube("Cube", shader_byte_code, size_shader);
	cube->setPosition(1.5, -1, 1.5);
	cube->setScale(0.5, 2, 0.5);
	this->objList.push_back((cube));

	Cube* cube1 = new Cube("Cube1", shader_byte_code, size_shader);
	cube1->setPosition(-1.5, -1, 1.5);
	cube1->setScale(0.5, 2, 0.5);
	this->objList.push_back((cube1));

	Cube* cube2 = new Cube("Cube2", shader_byte_code, size_shader);
	cube2->setPosition(1.5, -1, -1.5);
	cube2->setScale(0.5, 2, 0.5);
	this->objList.push_back((cube2));

	Cube* cube3 = new Cube("Cube3", shader_byte_code, size_shader);
	cube3->setPosition(-1.5, -1, -1.5);
	cube3->setScale(0.5, 2, 0.5);
	this->objList.push_back((cube3));

	Plane* plane = new Plane("Plane", shader_byte_code, size_shader);
	this->objList.push_back((plane));
	plane->setScale(4, 0.5f, 4);

	//Release Compiled Shader
	GraphicsEngine::get()->releaseCompiledShader();

	//Pixel Shader
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);

	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;

	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//UPDATE CAMERA
	SceneCameraHandler::getInstance()->update(EngineTime::getDeltaTime(), width, height);

	//FINALLY DRAW THE OBJECTS
	for(int i = 0; i < objList.size(); i++)
	{
		objList[i]->update(EngineTime::getDeltaTime());
		objList[i]->draw(width, height, this->m_vs, this->m_ps);
	}

	m_swap_chain->present(true);

	//FOR ANIMATION
	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();
	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();

	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{

}

void AppWindow::onKeyUp(int key)
{

}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);

	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{

}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{

}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{

}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{

}