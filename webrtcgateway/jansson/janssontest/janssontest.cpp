// janssontest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "jansson.h"

#pragma comment(lib, "..\\libjansson.dll.a")

int _tmain(int argc, _TCHAR* argv[])
{
	json_t *s = json_object();
	json_object_set_new(s, "id", json_integer(1));
	json_object_set_new(s, "ready", json_integer(2));
	json_object_set_new(s, "disabled", json_false());
	printf("ok");
	return 0;
}

