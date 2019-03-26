#include <iostream>
#include <string>
#include "Framework/Common/AssetLoader.h"
#include "Framework/Common/MemoryManager.h"
#include "Framework/Parser/OGEXParser.h"

using namespace Engine;
using namespace std;

namespace Engine {
	MemoryManager* g_pMemoryManager = new MemoryManager();
	AssetLoader*   g_pAssetLoader = new AssetLoader();
}

template<typename T>
static ostream& operator<<(ostream& out, unordered_map<string, shared_ptr<T>> map)
{
	for (auto p : map)
	{
		out << *p.second << endl;
	}

	return out;
}

int main(int, char**)
{
	g_pMemoryManager->Initialize();
	g_pAssetLoader->Initialize();

	string ogex_text = g_pAssetLoader->SyncOpenAndReadTextFileToString("Scene/Example.ogex");

	OgexParser* ogex_parser = new OgexParser();
	unique_ptr<Scene> pScene = ogex_parser->Parse(ogex_text);
	delete ogex_parser;

	cout << "Dump of Scene Graph" << endl;
	cout << "---------------------------" << endl;
	cout << *pScene->SceneGraph << endl;

	cout << "Dump of Cameras" << endl;
	cout << "---------------------------" << endl;
	cout << pScene->Cameras << endl;

	cout << "Dump of Lights" << endl;
	cout << "---------------------------" << endl;
	cout << pScene->Lights << endl;

	cout << "Dump of Geometries" << endl;
	cout << "---------------------------" << endl;
	cout << pScene->Geometries << endl;

	cout << "Dump of Materials" << endl;
	cout << "---------------------------" << endl;
	cout << pScene->Materials << endl;

	g_pAssetLoader->Finalize();
	g_pMemoryManager->Finalize();

	delete g_pAssetLoader;
	delete g_pMemoryManager;

	return 0;
}

