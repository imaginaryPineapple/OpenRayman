#ifdef _WIN32
#include <platform/data_extractor/windows.h>
#include <mingw.thread.h>

namespace openrayman
{
	data_extractor_dialog_windows::data_extractor_dialog_windows() :
		m_progress(0),
		m_wants_cancel(false),
		m_dirty(false),
		m_last_time(m_backend_specifics.time())
	{
		CoCreateInstance(CLSID_ProgressDialog, nullptr, CLSCTX_INPROC_SERVER, IID_IProgressDialog, (void**)&m_progress_dialog);
		m_progress_dialog->SetTitle(L"OpenRayman");
		m_progress_dialog->SetLine(1, L"", true, nullptr);
		m_progress_dialog->SetLine(2, L"", true, nullptr);
		m_progress_dialog->SetLine(3, L"", true, nullptr);
		m_progress_dialog->StartProgressDialog(nullptr, nullptr, PROGDLG_NORMAL | PROGDLG_NOTIME, nullptr);
		m_progress_dialog->SetProgress(0, 100);
	}

	data_extractor_dialog_windows::~data_extractor_dialog_windows()
	{
		m_progress_dialog->StopProgressDialog();
		m_progress_dialog->Release();
	}

	void data_extractor_dialog_windows::do_events()
	{
		m_wants_cancel = m_progress_dialog->HasUserCancelled();
		if(m_dirty.load())
		{
			m_progress_dialog->SetLine(1, std::wstring(m_title.begin(), m_title.end()).c_str(), true, nullptr);
			m_progress_dialog->SetLine(2, std::wstring(m_subtitle.begin(), m_subtitle.end()).c_str(), true, nullptr);
			m_progress_dialog->SetLine(3, std::wstring(m_message.begin(), m_message.end()).c_str(), true, nullptr);
			// the progress bar doesn't move on Windows 10 unless this is here
			if(m_backend_specifics.time() - m_last_time > 0.3)
			{
				m_progress_dialog->SetProgress((int)(m_progress * 100), 100);
				m_last_time = m_backend_specifics.time();
			}
			m_dirty.store(false);
		}
		MSG msg;
		while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
        	DispatchMessage(&msg);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	void data_extractor_dialog_windows::set_title(const std::string& title)
	{
		m_title = title;
		m_dirty.store(true);
	}

	void data_extractor_dialog_windows::set_subtitle(const std::string& subtitle)
	{
		m_subtitle = subtitle;
		m_dirty.store(true);
	}

	void data_extractor_dialog_windows::set_message(const std::string& msg)
	{
		m_message = msg;
		m_dirty.store(true);
	}

	void data_extractor_dialog_windows::set_progress(float progress)
	{
		m_progress = progress;
		m_dirty.store(true);
	}
}
#endif
