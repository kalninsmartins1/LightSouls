#include "VfxManager.h"

/* Class implementation details
 * Requirements:
 *  - Be generic data driven.
 *	- Have 1 Vfx for every defined event
 *	- When an event is triggered spawn the vfx in container.
 * Solution:
 *	- Store events to vfx map
 *	- When event is triggered use the map to figure out which one to trigger.
 *	- Events also need to have information about the position for vfx to spawn at.
*/

VfxManager VfxManager::Create(const String& pathToXML)
{

}
