#ifndef SPRINGLOBBY_HEADERGUARD_IBATTLE_H
#define SPRINGLOBBY_HEADERGUARD_IBATTLE_H


#include <wx/string.h>
#include <wx/event.h>

#include "iunitsync.h"
#include "user.h"
#include "mmoptionswrapper.h"
#include "userlist.h"
#include "tdfcontainer.h"


const unsigned int DEFAULT_SERVER_PORT = 8452;
const unsigned int DEFAULT_EXTERNAL_UDP_SOURCE_PORT = 16941;

class IBattle;
class wxTimer;
class wxTimerEvent;

struct BattleStartRect
{
    BattleStartRect()
    {
        toadd = false;
        todelete = false;
        exist = false;
        toresize = false;
    }
    bool toadd;
    bool todelete;
    bool toresize;
    bool exist;

    bool IsOk()
    {
        return exist && !todelete;
    }

    int ally;
    int top;
    int left;
    int right;
    int bottom;
};


enum NatType
{
		NAT_None = 0,
		NAT_Hole_punching,
		NAT_Fixed_source_ports
};

enum RankLimitType
{
		rank_limit_none = 0,
		rank_limit_autospec,
		rank_limit_autokick
};


enum BattleType
{
		BT_Played,
		BT_Replay,
		BT_Savegame
};


struct BattleOptions
{
	BattleOptions() :
		battleid(-1),islocked(false),battletype(BT_Played),ispassworded(false),rankneeded(0),isproxy(false),lockexternalbalancechanges(false),ranklimittype(rank_limit_autospec),
		nattype(NAT_None),port(DEFAULT_SERVER_PORT),externaludpsourceport(DEFAULT_EXTERNAL_UDP_SOURCE_PORT),internaludpsourceport(DEFAULT_EXTERNAL_UDP_SOURCE_PORT),maxplayers(0),spectators(0),
		guilistactiv(false) {}

	int battleid;
	bool islocked;
	BattleType battletype;
	bool ispassworded;
	int rankneeded;
	bool isproxy;
	bool lockexternalbalancechanges;
	bool ranklimittype;

	wxString founder;

	NatType nattype;
	unsigned int port;
	wxString ip;
	unsigned int externaludpsourceport;
	unsigned int internaludpsourceport;

	unsigned int maxplayers;
	unsigned int spectators;

	wxString maphash;
	wxString modhash;

	wxString description;
	wxString mapname;
	wxString modname;

	bool guilistactiv;
};

class IBattle: public UserList, public wxEvtHandler
{
public:

    IBattle();
    virtual ~IBattle();

    /** @name Constants
     * @{
     */
		enum HostInfo
		{
			HI_None = 0,
			HI_Map = 1,
			HI_Locked = 2,
			HI_Spectators = 4,
			HI_StartResources = 8,
			HI_MaxUnits = 16,
			HI_StartType = 32,
			HI_GameType = 64,
			HI_Options = 128,
			HI_StartRects = 256,
			HI_Restrictions = 512,
			HI_Map_Changed = 1024,
			HI_Mod_Changed = 2048,
			HI_User_Positions  = 4096,
			HI_Send_All_opts  = 8192
		};

    /**@}*/

    /** @name Enums
     * @{
     */


    enum BalanceType
    {
        balance_divide,
        balance_random
    };

    enum StartType
    {
        ST_Fixed = 0,
        ST_Random = 1,
        ST_Choose = 2,
        ST_Pick = 3
    };

    enum GameType
    {
        GT_ComContinue = 0,
        GT_ComEnds = 1,
        GT_Lineage = 2
    };


		struct TeamInfoContainer
		{
				bool exist;
				int TeamLeader;
				int StartPosX;
				int StartPosY;
				int AllyTeam;
				wxColor RGBColor;
				wxString SideName;
				int Handicap;
				int SideNum;
		};

		struct AllyInfoContainer
		{
				bool exist;
				int NumAllies;
				int StartRectLeft;
				int StartRectTop;
				int StartRectRight;
				int StartRectBottom;
		};


    /**@}*/

    virtual void SetHostMap( const wxString& mapname, const wxString& hash );
    virtual void SetLocalMap( const UnitSyncMap& map );
    virtual const UnitSyncMap& LoadMap();
    virtual wxString GetHostMapName() const;
    virtual wxString GetHostMapHash() const;

    virtual void SetIsProxy( bool value );
    virtual bool IsProxy();

    virtual bool IsSynced();

    virtual bool IsFounderMe();
    virtual bool IsFounder( const User& user ) const;

    virtual int GetMyPlayerNum();

		virtual int GetPlayerNum( const User& user );

    virtual void SetHostMod( const wxString& modname, const wxString& hash );
    virtual void SetLocalMod( const UnitSyncMod& mod );
    virtual const UnitSyncMod& LoadMod();
    virtual wxString GetHostModName() const;
    virtual wxString GetHostModHash() const;

    virtual bool MapExists() const;
    virtual bool ModExists() const;

    virtual BattleStartRect GetStartRect( unsigned int allyno );
    User& OnUserAdded( User& user );
    void OnUserBattleStatusUpdated( User &user, UserBattleStatus status );
    void OnUserRemoved( User& user );

		bool IsEveryoneReady();

		void ForceSide( User& user, int side );
		void ForceAlly( User& user, int ally );
		void ForceTeam( User& user, int team );
		void ForceColour( User& user, const wxColour& col );
		void ForceSpectator( User& user, bool spectator );
		void SetHandicap( User& user, int handicap);
		void KickPlayer( User& user );


    virtual void AddStartRect( unsigned int allyno, unsigned int left, unsigned int top, unsigned int right, unsigned int bottom );
    virtual void RemoveStartRect( unsigned int allyno );
    virtual void ResizeStartRect( unsigned int allyno );
    virtual void StartRectRemoved( unsigned int allyno );
    virtual void StartRectResized( unsigned int allyno );
    virtual void StartRectAdded( unsigned int allyno );
    virtual void ClearStartRects();
    virtual unsigned int GetNumRects();
	virtual unsigned int GetLastRectIdx();
	virtual unsigned int GetNextFreeRectIdx();

    virtual int GetFreeTeamNum( bool excludeme = false );

    virtual User& GetMe() = 0;

    virtual void SendHostInfo( HostInfo update );
    virtual void SendHostInfo( const wxString& Tag );
		virtual void Update ( const wxString& Tag );

    virtual unsigned int GetNumBots() const;
    virtual User& OnBotAdded( const wxString& nick, const UserBattleStatus& bs );

    virtual UserPosition GetFreePosition();
    virtual int GetFreeAlly( bool excludeme = false );

    virtual void RestrictUnit( const wxString& unitname, int count = 0 );
    virtual void UnrestrictUnit( const wxString& unitname );
    virtual void UnrestrictAllUnits();
    virtual std::map<wxString,int> RestrictedUnits();

    virtual void OnUnitSyncReloaded();

    virtual OptionsWrapper& CustomBattleOptions()
    {
        return m_opt_wrap;
    }

    virtual bool LoadOptionsPreset( const wxString& name );
    virtual void SaveOptionsPreset( const wxString& name );
    virtual wxString GetCurrentPreset();
    virtual void DeletePreset( const wxString& name );
    virtual wxArrayString GetPresetList();

    virtual std::vector<wxColour> &GetFixColoursPalette();
    virtual int GetClosestFixColour(const wxColour &col, const std::vector<int> &excludes, int &difference);
    virtual wxColour GetFixColour(int i);
    virtual wxColour GetFreeColour( User &for_whom ) const;
    wxColour GetNewColour() const;

    virtual int ColourDifference(const wxColour &a, const wxColour &b);

	User& GetFounder() const { return GetUser( m_opts.founder ); }

		bool IsFull() const { return GetMaxPlayers() == ( GetNumUsers() - GetSpectators() ); }

		virtual unsigned int GetNumPlayers() const;

		virtual int GetBattleId() const { return m_opts.battleid; }

		virtual bool GetGUIListActiv() const { return m_opts.guilistactiv; }
		virtual void SetGUIListActiv(bool Activ) { m_opts.guilistactiv = Activ; }

		virtual void SetInGame( bool ingame ) { m_ingame = ingame; }
		virtual bool GetInGame() const { return m_ingame; }

		virtual void SetBattleType( BattleType type ) { m_opts.battletype = type; }
		virtual BattleType GetBattleType() { return m_opts.battletype; }

		virtual void SetIsLocked( const bool islocked ) { m_opts.islocked = islocked; }
		virtual bool IsLocked() const { return m_opts.islocked; }
		virtual void SetIsPassworded( const bool ispassworded ) { m_opts.ispassworded = ispassworded; }
		virtual bool IsPassworded() const { return m_opts.ispassworded; }

		virtual void SetNatType( const NatType nattype ) { m_opts.nattype = nattype; }
		virtual NatType GetNatType() const { return m_opts.nattype; }
		virtual void SetHostPort( unsigned int port) { m_opts.port = port; }

		virtual void SetMyExternalUdpSourcePort(unsigned int port){m_opts.externaludpsourceport=port;}
		virtual unsigned int GetMyExternalUdpSourcePort(){return m_opts.externaludpsourceport;}

		virtual void SetMyInternalUdpSourcePort(unsigned int port){m_opts.internaludpsourceport=port;}
		virtual unsigned int GetMyInternalUdpSourcePort(){return m_opts.internaludpsourceport;}

		virtual int GetHostPort() const { return m_opts.port; }
		virtual void SetFounder( const wxString& nick ) { m_opts.founder = nick; }
		virtual void SetHostIp( const wxString& ip ) { m_opts.ip = ip; }
		virtual wxString GetHostIp() const { return m_opts.ip; }

		virtual void SetMaxPlayers( const int& maxplayers ) { m_opts.maxplayers = maxplayers; }
		virtual unsigned int GetMaxPlayers() const { return m_opts.maxplayers; }
		virtual void SetSpectators( const int& spectators ) { m_opts.spectators = spectators; }
		virtual int GetSpectators() const { return m_opts.spectators; }

		virtual void SetRankNeeded( const int& rankneeded ) { m_opts.rankneeded = rankneeded; }
		virtual int GetRankNeeded() const { return m_opts.rankneeded; }

		// virtual void SetMapHash( const wxString& maphash ) { m_opts.maphash = maphash; }
		// virtual void SetMapname( const wxString& map ) { m_opts.mapname = map; }
		virtual void SetDescription( const wxString& desc ) { m_opts.description = desc; }
		virtual wxString GetDescription() const { return m_opts.description; }
		// virtual void SetModname( const wxString& mod ) { m_opts.modname = mod; }


		typedef std::map<wxString, User> UserVec;
		typedef UserVec::const_iterator UserVecCIter;
		typedef UserVec::iterator UserVecIter;

		void SetBattleOptions( const BattleOptions& options ) { m_opts = options; }

		virtual void OnSelfLeftBattle();

		/// replay&savegame parsing stuff
		typedef std::map<int, TeamInfoContainer> TeamVec;
		typedef TeamVec::const_iterator TeamVecCIter;
		typedef TeamVec::iterator TeamVecIter;

		typedef std::map<int, AllyInfoContainer> AllyVec;
		typedef AllyVec::const_iterator AllyVecCIter;
		typedef AllyVec::iterator AllyVecIter;

		TeamVec GetParsedTeamsVec() { return m_parsed_teams; }
		AllyVec GetParsedAlliesVec() { return m_parsed_allies; }

		void SetParsedTeamsVec( const TeamVec& t ) { m_parsed_teams = t; }
		void SetParsedAlliesVec( const AllyVec& a ) { m_parsed_allies = a; }

		const BattleOptions& GetBattleOptions() const { return m_opts; }

		bool Equals( const IBattle& other ) const { return m_opts.battleid == other.GetBattleOptions().battleid; }

		virtual void DisableHostStatusInProxyMode( bool value ) { m_generating_script = value; }

		virtual void UserPositionChanged( const User& usr );

		virtual void SetScript( const wxString& script ) { m_script = script; }
		virtual void AppendScriptLine( const wxString& line ) { m_script << line; }
		virtual void ClearScript() { m_script.Clear(); }
		virtual wxString GetScript() { return m_script; }

		virtual void SetPlayBackFilePath( const wxString& path ) { m_playback_file_path = path; }
		virtual wxString GetPlayBackFilePath() { return m_playback_file_path; }

		virtual void AddUserFromDemo( User& user );

		virtual void GetBattleFromScript( bool loadmapmod );

		virtual bool ShouldAutoStart();

		virtual void FixColours();
    virtual void Autobalance( BalanceType balance_type = balance_divide, bool clans = true, bool strong_clans = true, int allyteamsize = 0 );
    virtual void FixTeamIDs( BalanceType balance_type = balance_divide, bool clans = true, bool strong_clans = true, int controlteamsize = 0 );

		virtual void StartSpring() = 0;

		virtual void OnTimer( wxTimerEvent& event );

protected:

		void LoadScriptMMOpts( const wxString& sectionname, const PDataList& node );
		void LoadScriptMMOpts( const PDataList& node );


    bool m_map_loaded;
    bool m_mod_loaded;
    bool m_map_exists;
    bool m_mod_exists;
    UnitSyncMap m_local_map;
    UnitSyncMod m_local_mod;
    UnitSyncMap m_host_map;
    UnitSyncMod m_host_mod;

    std::map<wxString, int> m_restricted_units;

    OptionsWrapper m_opt_wrap;

    BattleOptions m_opts;

		bool m_ingame;

		bool m_generating_script;

		std::map<unsigned int,BattleStartRect> m_rects;

		std::map<wxString, time_t> m_ready_up_map; // player name -> time counting from join/unspect

		int m_players_ready;
		int m_players_sync;
		std::map<int, int> m_teams_sizes; // controlteam -> number of people in
		std::map<int, int> m_ally_sizes; // allyteam -> number of people in

    wxString m_preset;

    bool m_is_self_in;
    UserVec m_internal_bot_list;

    /// replay&savegame stuff
    wxString m_script;
    wxString m_playback_file_path;
    TeamVec m_parsed_teams;
    AllyVec m_parsed_allies;
		UserVec m_internal_user_list; /// to store users from savegame/replay

		wxTimer* m_timer;

		DECLARE_EVENT_TABLE()
};

#endif // SPRINGLOBBY_HEADERGUARD_IBATTLE_H

/**
    This file is part of SpringLobby,
    Copyright (C) 2007-09

    springsettings is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as published by
    the Free Software Foundation.

    springsettings is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SpringLobby.  If not, see <http://www.gnu.org/licenses/>.
**/

