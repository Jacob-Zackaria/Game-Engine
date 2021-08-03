#ifndef TEXTURE_LINK_H
#define TEXTURE_LINK_H

namespace Azul
{
	class TextureLink
	{
	public:
		TextureLink()
		{
			this->next = 0;
			this->prev = 0;
		}

		virtual ~TextureLink()
		{

		}

		TextureLink* next;
		TextureLink* prev;
	};

}

#endif