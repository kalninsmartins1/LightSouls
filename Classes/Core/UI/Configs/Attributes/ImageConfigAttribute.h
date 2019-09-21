/*!
 * \Author	Martins Kalnins
 * \Date	17.09.2019
 * \Brief	Element configuration for basic element
 *			 that contains image.
*/

#pragma once

#include "IConfigAttribute.h"
#include "Classes/Core/String/String.h"

namespace UI
{
	class ImageConfigAttribute : public IConfigAttribute
	{
	public:
		static const String&						GetType();

		virtual int									GetPriority() const override;
		const String&								GetPathToImage() const;
		virtual std::unique_ptr<IConfigAttribute>	Clone() const override;

		void SetPathToImage(const String& pathToSprite);

		virtual void Apply(Element& target) const override;

	private:
		static String	s_type;
		String			m_pathToImage;
	};
}
