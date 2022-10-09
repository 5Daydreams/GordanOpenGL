#pragma once

class Test
{
public:
	Test() {}
	virtual ~Test() {}

	virtual void OnStartup(){}
	virtual void OnUpdate(float deltaTime) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}
};

class Test2
{
public:
	Test2();
	virtual ~Test2();

	virtual void OnStartup();
	virtual void OnUpdate(float deltaTime);
	virtual void OnRender();
	virtual void OnImGuiRender();
};
