#ifdef _WIN32
#ifndef DATA_EXTRACTOR_DIALOG_WINDOWS_H
#define DATA_EXTRACTOR_DIALOG_WINDOWS_H

#include <platform/standalone_backend_specifics.h>
#include <platform/data_extractor/data_extractor_dialog.h>
#include <windows.h>
#include <shlobj.h>
#include <atomic>

namespace openrayman
{
	class data_extractor_dialog_windows : public data_extractor_dialog_base
	{
public:
		data_extractor_dialog_windows();
		~data_extractor_dialog_windows();

		void do_events() override;
		void set_title(const std::string& title) override;
		void set_subtitle(const std::string& subtitle) override;
		void set_message(const std::string& msg) override;
		void set_progress(float progress) override;

		inline void set_cancel()
		{
		 	m_wants_cancel = true;
		}

		inline bool wants_cancel() const override
		{
			return m_wants_cancel;
		}
private:
		std::atomic<bool> m_dirty;
		bool m_wants_cancel;

		std::string m_message, m_title, m_subtitle;
		float m_progress;

		IProgressDialog* m_progress_dialog;

		standalone_backend_specifics m_backend_specifics;
		double m_last_time;
	};
}

#endif
#endif
