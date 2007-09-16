#ifndef SPRINGLOBBY_HEADERGUARD_UNITSYNCTHREAD_H
#define SPRINGLOBBY_HEADERGUARD_UNITSYNCTHREAD_H

#include <wx/thread.h>
#include <wx/event.h>


class wxArrayString;
class Ui;

typedef wxChar JobType;
#define JT_PAUSE ' '
#define JT_MAPINFO 'i'
#define JT_MINIMAP 'm'
#define JT_UNITS 'u'


BEGIN_DECLARE_EVENT_TYPES()
DECLARE_LOCAL_EVENT_TYPE( wxEVT_UNITSYNC_CACHE, 1 )
END_DECLARE_EVENT_TYPES()


class UnitSyncThread: public wxThread, public wxEvtHandler
{

  public:

    UnitSyncThread( Ui& ui );
    ~UnitSyncThread();

    void Init() {
      Create();
      SetPriority( WXTHREAD_MIN_PRIORITY );
      Run();
    }

    void* Entry();
    void OnExit();

    void AddMapInfoOrder( const wxString& map, int startdelay = -1 );
    void AddMinimapOrder( const wxString& map, int startdelay = -1 );
    void AddModUnitsOrder( const wxString& mod, int startdelay = -1 );

    void Pause();
    void Resume();

    void OnMinimapCached( wxCommandEvent& event );

  protected:

    void _CacheLoop();
    bool _GetNextJob( JobType& jobtype, wxString& params );

    void _AddJob( const wxChar& prefix, const wxString& params );

    void _DoMapInfoJob( const wxString& map );
    void _DoMinimapJob( const wxString& map );
    void _DoUnitsJob( const wxString& mod );

    Ui& m_ui;

    int m_delay;
    bool m_should_terminate;
    bool m_should_pause;

    wxArrayString m_orders;
    wxCriticalSection m_lock;
    wxString m_last_job;


    DECLARE_EVENT_TABLE()

};


enum {
  CACHE_MINIMAP = wxID_HIGHEST,
  CACHE_MAPINFO,
  CACHE_UNITS
};


#endif // SPRINGLOBBY_HEADERGUARD_UNITSYNCTHREAD_H
