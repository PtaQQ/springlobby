#ifndef SPRINGLOBBY_HEADERGUARD_BATTLEROOMLISTCTRL_H
#define SPRINGLOBBY_HEADERGUARD_BATTLEROOMLISTCTRL_H

#include "customlistctrl.h"
#include "usermenu.h"

class User;
class Battle;
class Ui;
struct BattleBot;
//class wxMenuItem;
class wxIcon;


struct item_content {
  bool is_bot;
  void* data;
};

/** \brief display participants of battle and their info (ally,team,color,cpu...)
 * \todo DOCMEMORE */
class BattleroomListCtrl : public customListCtrl
{
  public:
    BattleroomListCtrl( wxWindow* parent, Battle& battle, Ui& ui );
     ~BattleroomListCtrl();

    void Sort();

    void AddUser( User& user );
    void RemoveUser( User& user );
    void UpdateUser( const int& index );
    void UpdateUser( User& user );
    void UpdateList();

    void AddBot( BattleBot& bot );
    void RemoveBot( BattleBot& bot );
    void UpdateBot( BattleBot& bot );
    void UpdateBot( const int& index );

    int GetUserIndex( User& user );
    int GetBotIndex( BattleBot& bot );

    void OnListRightClick( wxListEvent& event );
    void OnColClick( wxListEvent& event );
    void OnTeamSelect( wxCommandEvent& event );
    void OnAllySelect( wxCommandEvent& event );
    void OnColourSelect( wxCommandEvent& event );
    void OnSideSelect( wxCommandEvent& event );
    void OnHandicapSelect( wxCommandEvent& event );
    void OnSpecSelect( wxCommandEvent& event );

    void OnKickPlayer( wxCommandEvent& event );
    void OnRingPlayer( wxCommandEvent& event );
    void OnUserMenuCreateGroup( wxCommandEvent& event );
    void OnUserMenuDeleteFromGroup( wxCommandEvent& event );
    void OnUserMenuAddToGroup( wxCommandEvent& event );
    virtual void SetTipWindowText( const long item_hit, const wxPoint position);
    virtual void HighlightItem( long item );

  protected:
    static int wxCALLBACK CompareStatusUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareStatusDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareSideUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareSideDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareColorUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareColorDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareCountryUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareCountryDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareRankUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareRankDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareNicknameUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareNicknameDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareTeamUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareTeamDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareAllyUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareAllyDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareCpuUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareCpuDOWN(long item1, long item2, long sortData);
    static int wxCALLBACK CompareHandicapUP(long item1, long item2, long sortData);
    static int wxCALLBACK CompareHandicapDOWN(long item1, long item2, long sortData);
    wxString GetCellContentsString( long row_number, int column );

    wxString GetSelectedUserNick();

    struct {
      int col;
      bool direction;
    } m_sortorder[4];

    Battle& m_battle;


    typedef SL_GENERIC::UserMenu<BattleroomListCtrl> UserMenu;
    UserMenu* m_popup;

    User* m_sel_user;
    BattleBot* m_sel_bot;
    std::vector<item_content> items;

    wxMenu* m_sides;
    wxMenuItem* m_spec_item;

    wxMenuItem* m_handicap_item;

    Ui& m_ui;
    static Ui* m_ui_for_sort;

    DECLARE_EVENT_TABLE();

};

enum
{
  BRLIST_LIST = wxID_HIGHEST,
  BRLIST_TEAM,
  BRLIST_ALLY = BRLIST_TEAM + 1000,
  BRLIST_COLOUR = BRLIST_ALLY + 1000,
  BRLIST_SIDE = BRLIST_COLOUR +1000,
  BRLIST_HANDICAP = BRLIST_SIDE +1000,
  BRLIST_SPEC,
  BRLIST_KICK,
  BRLIST_RING,
  BRLIST_ADDTOGROUP
};

#endif // SPRINGLOBBY_HEADERGUARD_BATTLEROOMLISTCTRL_H
