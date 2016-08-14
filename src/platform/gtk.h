#ifndef GTK_H
#define GTK_H

#include <gtkmm.h>

namespace openrayman
{
	static bool gtk_has_init = false;

	inline bool can_use_gtk()
	{
		if(gtk_has_init)
			return true;

		return (gtk_has_init = gtk_init_check(0, nullptr));
	}
}

#endif
