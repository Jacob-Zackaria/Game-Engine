#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Texture.h"

namespace Azul
{
	class TextureManager
	{
	public:
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator = (const TextureManager&) = delete;
		~TextureManager() = default;

		static void Create();
		static void Destroy();
		static void Add(const char* const _assetName, const Texture::Name _name);
		static void AddBuff(const char* const buffer, const char* const asset_name, const Texture::Name _name);
		static void Remove(const Texture::Name _name);

		//update reference count.
		static void Load(const Texture::Name _name);
		static void UnLoad(const Texture::Name _name);

		static void DeleteAllTextures();

		static Texture* Find(const Texture::Name _name);
		static GLuint FindID(const Texture::Name _name);
		static void Dump();

	private:
		// methods
		TextureManager();
		static TextureManager* privGetInstance();
		void DeleteTexture(unsigned int);
		// helper private methods
		void privLoadTexture(const char* const _assetName, GLuint*& textureID, int& width, int& height);
		void privLoadBuffTexture(const char* const buffer, GLuint*& textureID, int& width, int& height);
		bool privLoadTGATexture(const char* szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& width, int& height);
		bool privLoadTGABuffTexture(const char* buffer, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& width, int& height);
		void privAddToFront(TextureLink* node, TextureLink*& head);
		void privRemove(TextureLink* pNewData, TextureLink*& pHead);
		GLbyte* gltReadTGABits(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);
		GLbyte* gltReadTGABuffBits(const char* buffer, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);

	private:

		TextureLink* active;
	};
}
#endif