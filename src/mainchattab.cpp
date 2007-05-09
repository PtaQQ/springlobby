/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */


//
// Class: MainChatTab
//
// Created on: Tue May  1 13:32:49 2007
//

#include "mainchattab.h"


MainChatTab::MainChatTab( wxWindow* parent ) 
: wxPanel( parent, -1, wxDefaultPosition, wxDefaultSize, 0, wxPanelNameStr )
{

  m_main_sizer = new wxBoxSizer( wxVERTICAL );
  
  m_chat_tabs = new wxNotebook( this, -1, wxDefaultPosition, wxDefaultSize, wxLB_TOP );
  
  m_server_chat = new ChatPanel( m_chat_tabs, false );
  m_main_chat = new ChatPanel( m_chat_tabs, true );
  
  m_chat_tabs->AddPage( m_server_chat, _T("Server"), true, -1 );
  m_chat_tabs->AddPage( m_main_chat, _T("Main"), false, -1 );
  
  m_main_sizer->Add( m_chat_tabs, 1, wxEXPAND, 0 );
  
  SetSizer( m_main_sizer );
  Layout();
  m_main_sizer->SetSizeHints( this );
}


MainChatTab::~MainChatTab()
{
  
}


