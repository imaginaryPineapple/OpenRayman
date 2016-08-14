#ifndef DATA_EXTRACTOR_DIALOG_H
#define DATA_EXTRACTOR_DIALOG_H

#include <info.h>
#include <string>

namespace openrayman
{
	class data_extractor_dialog_base
	{
public:
		virtual ~data_extractor_dialog_base()
		{
		}

		// Process pending events.
		virtual void do_events() = 0;

		// Set the title displayed to the user.
		// Called from extraction thread.
		virtual void set_title(const std::string& title) = 0;

		// Set the sub title displayed to the user.
		// Called from extraction thread.
		virtual void set_subtitle(const std::string& subtitle) = 0;

		// Set the message displayed to the user.
		// Called from extraction thread.
		virtual void set_message(const std::string& msg) = 0;

		// Set the progress bar to a value between [0, 1].
		// Called from extraction thread.
		virtual void set_progress(float progress) = 0;

		// Returns true if the user requested a cancellation of the extraction process.
		virtual bool wants_cancel() const = 0;
	};
}

#endif
