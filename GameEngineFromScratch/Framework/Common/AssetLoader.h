#pragma once

#include <cstdio>
#include <string>
#include <utility>
#include <vector>
#include "Buffer.h"
#include "Framework/Interface/IRuntimeModule.h"

namespace Engine {

	class AssetLoader : public IRuntimeModule {
	public:
		typedef void* AssetFilePtr;

		enum struct AssetOpenMode {
			MY_OPEN_TEXT = 0, /// Open In Text Mode
			MY_OPEN_BINARY = 1, /// Open In Binary Mode 
		};

		enum struct AssetSeekBase {
			MY_SEEK_SET = 0, /// SEEK_SET
			MY_SEEK_CUR = 1, /// SEEK_CUR
			MY_SEEK_END = 2  /// SEEK_END
		};

	private:
		std::vector<std::string> m_strSearchPath;

	public:
		AssetLoader() = default;
		virtual ~AssetLoader() = default;

		virtual int Initialize();

		virtual void Finalize();

		virtual void Tick();

		bool AddSearchPath(const char* path);

		bool RemoveSearchPath(const char* path);

		bool FileExists(const char* filePath);

		AssetFilePtr OpenFile(const char* name, AssetOpenMode mode);

		Buffer SyncOpenAndReadText(const char* filePath);

		Buffer SyncOpenAndReadBinary(const char* filePath);

		size_t SyncRead(const AssetFilePtr& fp, Buffer& buf);

		void CloseFile(AssetFilePtr& fp);

		size_t GetSize(const AssetFilePtr& fp);

		int32_t Seek(AssetFilePtr fp, long offset, AssetSeekBase where);

		inline std::string SyncOpenAndReadTextFileToString(const char* fileName)
		{
			std::string result;
			Buffer buffer = SyncOpenAndReadText(fileName);
			char* content = reinterpret_cast<char*>(buffer.m_pData);

			if (content) {
				result = std::string(std::move(content));
			}

			return result;
		}
	};

	extern AssetLoader* g_pAssetLoader;

}
