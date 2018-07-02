#include "stdafx.h"
#include "Global.h"

Global::Global()
{
	Resources_base_path = utils::Get_exe_path() + "\\";  // Current *.exe path
	//Resources_base_path = "E:\\Demo_SP\\";  // Dev resources path

	game_root.Init();
}

Global::~Global()
{
}