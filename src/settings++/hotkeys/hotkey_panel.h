
#ifndef __TAB_HOTKEY_h__
#define __TAB_HOTKEY_h__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/scrolwin.h>

#include <list>
#include <vector>
#include <memory>

#include <wx/keybinder.h>

#include "hotkey_parser.h"


class hotkey_panel : public wxScrolledWindow
{
public:
	hotkey_panel(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Hotkeys"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0);
	~hotkey_panel(void);

	void SaveSettings();
	void UpdateControls(int=0/*unused*/);

private:
	wxKeyProfile buildNewProfile( const wxString& name, const wxString& description, bool readOnly );

	typedef std::list<wxTextCtrl*>			EditControlList;
	typedef std::list<wxStaticText*>		StaticControlList;

	EditControlList		textControls;
	StaticControlList	staticControls;

	wxButton*			m_button2;
	static wxCmd* cmdCreate(int id);
	//wxKeyProfileArray	m_profilesArray;
	wxKeyConfigPanel	m_keyConfigPanel;
	hotkey_parser		m_uikeys_manager;
};

#endif