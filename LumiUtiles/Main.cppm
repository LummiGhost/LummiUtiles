export module LumiUtiles:_Main;

import :Async;
import :Exception;
//import LumiUtiles:IO;

using namespace LumiUtiles::Async;
using namespace LumiUtiles::Exception;

export enum class Result {
	Success,
	Fail,
	ErrorAndRestore,
	Crash
};

/// case DLL_PROCESS_ATTACH:
/// case DLL_THREAD_ATTACH:
/// case DLL_THREAD_DETACH:
/// case DLL_PROCESS_DETACH:

export Result On_Process_Attach() {

}

export Result On_Process_Detach() {

}

export Result On_Thread_Attach() {

}

export Result On_Thread_Detach() {

}