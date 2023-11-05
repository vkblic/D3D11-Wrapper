/*
	CONTRIBUTORS:
		Sean Pesce

*/

#include "d3d11/main.h"


namespace d3d11 {

	HMODULE chain = NULL;
	FARPROC functions[func_count];

	//¹Ø¼ü´úÂë£º
	BOOL IsWow64()
	{

		typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

		LPFN_ISWOW64PROCESS fnIsWow64Process = nullptr;
		BOOL bIsWow64 = FALSE;

		//IsWow64Process is not available on all supported versions of Windows.
		//Use GetModuleHandle to get a handle to the DLL that contains the function
		//and GetProcAddress to get a pointer to the function if available.

		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
			GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

		if (NULL != fnIsWow64Process)
		{
			if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
			{
				//handle error
			}
		}

		return bIsWow64;
	}

	void hook_exports(HMODULE h_module)
	{
		if (IsWow64())
		{
			chain = LoadLibrary("C:\\Windows\\SysWOW64\\d3d11.dll");
		}
		else
		{
			chain = LoadLibrary("C:\\Windows\\System32\\d3d11.dll");
		}

		if (!chain)
		{
			MessageBox(NULL, "Unable to locate original d3d11.dll (or compatible library to chain)", "ERROR: Failed to load original d3d11.dll", NULL);
			exit(0);
		}
		//const int result = MessageBox(NULL, "######dx11 wrapper: try loading renderdoc", "try loading renderdoc", MB_YESNOCANCEL);

		PCHAR pszBuff = new CHAR[MAX_PATH];

		RtlZeroMemory(pszBuff, MAX_PATH);

		auto dwRet = GetModuleFileName(h_module, pszBuff, MAX_PATH);

		const int result = MessageBox(NULL, "######dx11 wrapper: try loading renderdoc", pszBuff, MB_YESNOCANCEL);
		int count = 0;
		for (int i = 0; i < d3d11::func_count; i++)
		{
			FARPROC func = GetProcAddress(chain, func_names[i]);
			if (func)
			{
				count++;
			}
			functions[i] = func;
		}


		if (result == IDYES)
		{


			//std::string rdcpath = "C:\\RenderDoc\\RenderDoc1.14\\renderdoc.dll";
			std::string rdcpath = "C:\\Program Files\\RenderDoc\\renderdoc.dll";
			HMODULE renderdoc = LoadLibrary(rdcpath.c_str());
			if (!renderdoc) {

				MessageBox(NULL, "Unable to locate renderdoc v1.14(C:\\Program Files\\RenderDoc\\renderdoc.dll)(or compatible library to chain), try v1.24", "ERROR: Failed to load renderdoc.dll", NULL);
				rdcpath = "C:\\RenderDoc\\RenderDoc1.24\\renderdoc.dll";
				renderdoc = LoadLibrary(rdcpath.c_str());
				if (!renderdoc)
				{
					MessageBox(NULL, "Unable to locate renderdoc v1.24(C:\\RenderDoc\\RenderDoc1.24\\renderdoc.dll)(or compatible library to chain), abort!", "ERROR: Failed to load renderdoc.dll", NULL);
					exit(0);
				}
			}
			else
			{
				MessageBox(NULL, "######dx11 wrapper :load renderdoc.dll done", "load renderdoc.dll done!", NULL);
			}
		}
	}

} // namespace d3d11



