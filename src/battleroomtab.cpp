/* Copyright (C) 2007 The SpringLobby Team. All rights reserved. */
//
// Class: BattleRoomTab
//

#include <wx/splitter.h>
#include <wx/intl.h>
#include <wx/combobox.h>
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/sizer.h>

#include "battleroomtab.h"
#include "ui.h"
#include "iunitsync.h"
#include "user.h"
#include "battle.h"
#include "utils.h"
#include "battleroomlistctrl.h"
#include "chatpanel.h"
#include "mapctrl.h"
#include "uiutils.h"


BEGIN_EVENT_TABLE(BattleRoomTab, wxPanel)

  EVT_BUTTON ( BROOM_START, BattleRoomTab::OnStart )
  EVT_BUTTON ( BROOM_LEAVE, BattleRoomTab::OnLeave )
  EVT_CHECKBOX( BROOM_IMREADY, BattleRoomTab::OnImReady )
  EVT_CHECKBOX( BROOM_SPEC, BattleRoomTab::OnImSpec )
  EVT_COMBOBOX( BROOM_TEAMSEL, BattleRoomTab::OnTeamSel )
  EVT_COMBOBOX( BROOM_ALLYSEL, BattleRoomTab::OnAllySel )
  EVT_COMBOBOX( BROOM_COLOURSEL, BattleRoomTab::OnColourSel )
  EVT_COMBOBOX( BROOM_SIDESEL, BattleRoomTab::OnSideSel )

END_EVENT_TABLE()

const wxString team_choices[] = { _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8"), _T("9"), _T("10"), _T("11"), _T("12"), _T("13"), _T("14"), _T("15"), _T("16") };

const wxString colour_choices[] = {
  _T("black"), _T("dark gray"), _T("dark blue"), _T("bright blue"), _T("dark green"),
  _T("bright green"), _T("dark cyan"), _T("bright cyan"), _T("dark red"), _T("bright red"),
  _T("dark magenta"), _T("bright magenta"), _T("dark yellow"), _T("bright yellow"),
  _T("light gray"), _T("inky blue")
};

const int colour_values[][3] = { {0,0,0}, {128, 128, 128}, {0, 0, 128}, {0, 0, 255},
  {0, 128, 0}, {0, 255, 0}, {0, 128, 128}, {0, 255, 255}, {128, 0, 0}, {255, 0, 0},
  {128, 0, 128}, {255, 0, 255}, {128, 128, 0}, {255, 255, 0}, {192, 192, 192}, {0, 220, 250}
};


BattleRoomTab::BattleRoomTab( wxWindow* parent, Ui& ui, Battle& battle ) : wxPanel( parent, -1 ),m_ui(ui), m_battle(battle)
{
  // Create all widgets
  m_splitter = new wxSplitterWindow( this, -1, wxDefaultPosition, wxSize(100, 60) );

  m_player_panel = new wxPanel( m_splitter , -1 );
  m_team_sel = new wxComboBox( m_player_panel, BROOM_TEAMSEL, _T("1"), wxDefaultPosition, wxSize(40,CONTROL_HEIGHT), 16, team_choices );
  m_ally_sel = new wxComboBox( m_player_panel, BROOM_ALLYSEL, _T("1"), wxDefaultPosition, wxSize(40,CONTROL_HEIGHT), 16, team_choices );
  m_color_sel = new wxComboBox( m_player_panel, BROOM_COLOURSEL, _("black"), wxDefaultPosition, wxSize(100,CONTROL_HEIGHT), 16, colour_choices );
  m_side_sel = new wxComboBox( m_player_panel, BROOM_SIDESEL, _T(""), wxDefaultPosition, wxSize(80,CONTROL_HEIGHT) );

  for ( int i = 0; i < usync()->GetSideCount( battle.opts().modname ); i++ ) {
    m_side_sel->Append( WX_STRING(usync()->GetSideName( battle.opts().modname, i )) );
  }

  m_team_lbl = new wxStaticText( m_player_panel, -1, _("Team") );
  m_ally_lbl = new wxStaticText( m_player_panel, -1, _("Ally") );
  m_color_lbl = new wxStaticText( m_player_panel, -1, _("Color") );
  m_side_lbl = new wxStaticText( m_player_panel, -1, _("Side") );

//  UnitSyncMap map = usync()->GetMap( battle.opts().mapname, true );
  m_map_lbl = new wxStaticText( this, -1, RefineMapname( WX_STRING(battle.opts().mapname) ) );
  m_size_lbl = new wxStaticText( this, -1, _("") );
  m_wind_lbl = new wxStaticText( this, -1, _("") );
  m_tidal_lbl = new wxStaticText( this, -1, _("") );

  m_minimap = new MapCtrl( this, 162, m_battle, true );

  m_players = new BattleroomListCtrl( m_player_panel );
  m_chat = new ChatPanel( m_splitter, m_ui, battle );

  m_command_line = new wxStaticLine( this, -1, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );

  m_leave_btn = new wxButton( this, BROOM_LEAVE, _("Leave"), wxDefaultPosition, wxSize(80,CONTROL_HEIGHT) );
  m_start_btn = new wxButton( this, BROOM_START, _("Start"), wxDefaultPosition, wxSize(80,CONTROL_HEIGHT) );

  m_ready_chk = new wxCheckBox( this, BROOM_IMREADY, _("I'm ready"), wxDefaultPosition, wxSize(80,CONTROL_HEIGHT) );
  m_spec_chk = new wxCheckBox( m_player_panel, BROOM_SPEC, _("Spectator"), wxDefaultPosition, wxSize(80,CONTROL_HEIGHT) );

  // Create Sizers
  m_players_sizer = new wxBoxSizer( wxVERTICAL );
  m_player_sett_sizer = new wxBoxSizer( wxHORIZONTAL );
  m_info_sizer = new wxBoxSizer( wxVERTICAL );
  m_top_sizer = new wxBoxSizer( wxHORIZONTAL );
  m_buttons_sizer = new wxBoxSizer( wxHORIZONTAL );
  m_info1_sizer = new wxBoxSizer( wxHORIZONTAL );
  m_main_sizer = new wxBoxSizer( wxVERTICAL );

  // Put widgets in place

  m_player_sett_sizer->Add( m_team_lbl, 0, wxEXPAND | wxALL, 2 );
  m_player_sett_sizer->Add( m_team_sel, 0, wxEXPAND | wxALL, 2 );
  m_player_sett_sizer->Add( m_ally_lbl, 0, wxEXPAND | wxALL, 2 );
  m_player_sett_sizer->Add( m_ally_sel, 0, wxEXPAND | wxALL, 2 );
  m_player_sett_sizer->Add( m_color_lbl, 0, wxEXPAND | wxALL, 2 );
  m_player_sett_sizer->Add( m_color_sel, 0, wxEXPAND | wxALL, 2 );
  m_player_sett_sizer->Add( m_side_lbl, 0, wxEXPAND | wxALL, 2 );
  m_player_sett_sizer->Add( m_side_sel, 0, wxEXPAND | wxALL, 2 );
  m_player_sett_sizer->Add( m_spec_chk, 0, wxEXPAND | wxALL, 2 );

  m_players_sizer->Add( m_players, 1, wxEXPAND );
  m_players_sizer->Add( m_player_sett_sizer, 0, wxEXPAND );

  m_player_panel->SetSizer( m_players_sizer );

  m_splitter->SplitHorizontally( m_player_panel, m_chat, 50 );

  m_info1_sizer->Add( m_wind_lbl, 1, wxEXPAND );
  m_info1_sizer->Add( m_size_lbl, 1, wxEXPAND );

  m_info_sizer->Add( m_minimap, 0, wxEXPAND );
  m_info_sizer->Add( m_map_lbl, 0, wxEXPAND );
  m_info_sizer->Add( m_info1_sizer, 0, wxEXPAND );
  m_info_sizer->Add( m_tidal_lbl, 0, wxEXPAND );

  m_top_sizer->Add( m_splitter, 1, wxEXPAND | wxALL, 2 );
  m_top_sizer->Add( m_info_sizer, 0, wxEXPAND | wxALL, 2 );

  m_buttons_sizer->Add( m_leave_btn, 0, wxEXPAND | wxALL, 2 );
  m_buttons_sizer->AddStretchSpacer();
  m_buttons_sizer->Add( m_ready_chk, 0, wxEXPAND | wxALL, 2 );
  m_buttons_sizer->Add( m_start_btn, 0, wxEXPAND | wxALL, 2 );

  m_main_sizer->Add( m_top_sizer, 1, wxEXPAND );
  m_main_sizer->Add( m_command_line, 0, wxEXPAND );
  m_main_sizer->Add( m_buttons_sizer, 0, wxEXPAND );

  SetSizer( m_main_sizer );
  Layout();

  UpdateBattleInfo();

  m_splitter->SetMinimumPaneSize( 100 );
  m_splitter->SetSashPosition( 200 );

  for ( user_map_t::size_type i = 0; i < battle.GetNumUsers(); i++ ) {
    m_players->AddUser( battle.GetUser( i ) );
  }

  if ( !IsHosted() ) {
    m_start_btn->Enable( false );
  }

}


BattleRoomTab::~BattleRoomTab()
{

}


bool BattleRoomTab::IsHosted()
{
  return m_battle.IsFounderMe();
}


void BattleRoomTab::UpdateBattleInfo()
{
  try {
    UnitSyncMap map = usync()->GetMap( m_battle.opts().mapname );
    if ( map.hash != m_map.hash ) map = m_map = usync()->GetMap( m_battle.opts().mapname, true );
    else map = m_map;
    m_map_lbl->SetLabel( RefineMapname( WX_STRING(map.name) ) );
    m_size_lbl->SetLabel( wxString::Format( _("Size: %.0fx%.0f"), map.info.width/512.0, map.info.height/512.0 ) );
    m_wind_lbl->SetLabel( wxString::Format( _("Wind: %d-%d"), map.info.minWind, map.info.maxWind) );
    m_tidal_lbl->SetLabel( wxString::Format( _("Tidal: %d"), map.info.tidalStrength) );
  } catch (...) {
    m_map_lbl->SetLabel( WX_STRING(m_battle.opts().mapname) );
    m_size_lbl->SetLabel( _("Size: ?x?") );
    m_wind_lbl->SetLabel( _("Wind: ?-?") );
    m_tidal_lbl->SetLabel( _("Tidal: ?") );
  }
  m_minimap->UpdateMinimap();
}


BattleroomListCtrl& BattleRoomTab::GetPlayersListCtrl()
{
  assert( m_players != 0);
  return *m_players;
}

void BattleRoomTab::UpdateUser( User& user )
{
  m_players->UpdateUser( user );
  if ( &user != &m_battle.GetMe() ) return;

  UserBattleStatus& bs = user.BattleStatus();
  m_team_sel->SetSelection( bs.team );
  m_ally_sel->SetSelection( bs.ally );
  m_side_sel->SetSelection( bs.side );
  m_spec_chk->SetValue( bs.spectator );
  m_ready_chk->SetValue( bs.ready );
  
  m_minimap->UpdateMinimap();
}

Battle& BattleRoomTab::GetBattle()
{
  return m_battle;
}

ChatPanel& BattleRoomTab::GetChatPanel()
{
  return *m_chat;
}


void BattleRoomTab::OnStart( wxCommandEvent& event )
{
  //ASSERT_LOGIC( IsHosted(), "This battle is not hosted by user." );
  m_ui.StartHostedBattle();
}


void BattleRoomTab::OnLeave( wxCommandEvent& event )
{
  m_battle.Leave();
}


void BattleRoomTab::OnImReady( wxCommandEvent& event )
{
  m_battle.SetImReady( m_ready_chk->GetValue() );
}


void BattleRoomTab::OnImSpec( wxCommandEvent& event )
{
  UserBattleStatus& bs = m_battle.GetMe().BattleStatus();
  bs.spectator = m_spec_chk->GetValue();
  //m_battle.GetMe().SetBattleStatus( bs );
  m_battle.SendMyBattleStatus();
}


void BattleRoomTab::OnTeamSel( wxCommandEvent& event )
{
  User& u = m_battle.GetMe();
  UserBattleStatus& bs = u.BattleStatus();
  m_team_sel->GetValue().ToULong( (unsigned long*)&bs.team );
  bs.team--;
  //u.SetBattleStatus( bs );
  m_battle.SendMyBattleStatus();
}


void BattleRoomTab::OnAllySel( wxCommandEvent& event )
{
  User& u = m_battle.GetMe();
  UserBattleStatus& bs = u.BattleStatus();
  m_ally_sel->GetValue().ToULong( (unsigned long*)&bs.ally );
  bs.ally--;
  //u.SetBattleStatus( bs );
  m_battle.SendMyBattleStatus();
}


void BattleRoomTab::OnColourSel( wxCommandEvent& event )
{
  User& u = m_battle.GetMe();
  UserBattleStatus& bs = u.BattleStatus();
  int i = m_color_sel->GetSelection();
  bs.color_r = colour_values[i][0];
  bs.color_g = colour_values[i][1];
  bs.color_b = colour_values[i][2];
  //u.SetBattleStatus( bs );
  m_battle.SendMyBattleStatus();
}


void BattleRoomTab::OnSideSel( wxCommandEvent& event )
{
  User& u = m_battle.GetMe();
  UserBattleStatus& bs = u.BattleStatus();
  bs.side = m_side_sel->GetSelection();
  //u.SetBattleStatus( bs );
  m_battle.SendMyBattleStatus();
}


void BattleRoomTab::OnUserJoined( User& user )
{
  m_chat->Joined( user );
  m_players->AddUser( user );
  if ( &user == &m_battle.GetMe() ) 
    m_players->SetItemState( m_players->GetUserIndex( user ), wxLIST_MASK_STATE, wxLIST_STATE_SELECTED );
}


void BattleRoomTab::OnUserLeft( User& user )
{
  m_chat->Parted( user, wxEmptyString );
  m_players->RemoveUser( user );
}


