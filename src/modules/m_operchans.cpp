/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd: (C) 2002-2010 InspIRCd Development Team
 * See: http://wiki.inspircd.org/Credits
 *
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#include "inspircd.h"

/* $ModDesc: Provides support for oper-only chans via the +O channel mode */

class OperChans : public SimpleChannelModeHandler
{
 public:
	/* This is an oper-only mode */
	OperChans(Module* Creator) : SimpleChannelModeHandler(Creator, "operonly", 'O') { fixed_letter = false; }
};

class ModuleOperChans : public Module
{
	OperChans oc;
 public:
	ModuleOperChans() : oc(this)
	{
		ServerInstance->Modules->AddService(oc);
		Implementation eventlist[] = { I_OnCheckBan, I_On005Numeric, I_OnUserPreJoin };
		ServerInstance->Modules->Attach(eventlist, this, 3);
	}

	ModResult OnUserPreJoin(User* user, Channel* chan, const std::string& cname, std::string &privs, const std::string &keygiven)
	{
		if (chan && chan->IsModeSet(&oc) && !IS_OPER(user))
		{
			user->WriteNumeric(ERR_CANTJOINOPERSONLY, "%s %s :Only IRC operators may join %s (+O is set)",
				user->nick.c_str(), chan->name.c_str(), chan->name.c_str());
			return MOD_RES_DENY;
		}
		return MOD_RES_PASSTHRU;
	}

	ModResult OnCheckBan(User *user, Channel *c, const std::string& mask)
	{
		if (mask[0] == 'O' && mask[1] == ':')
		{
			if (IS_OPER(user) && InspIRCd::Match(user->oper->name, mask.substr(2)))
				return MOD_RES_DENY;
		}
		return MOD_RES_PASSTHRU;
	}

	void On005Numeric(std::string &output)
	{
		ServerInstance->AddExtBanChar('O');
	}

	~ModuleOperChans()
	{
	}

	Version GetVersion()
	{
		return Version("Provides support for oper-only chans via the +O channel mode and 'O' extban", VF_VENDOR);
	}
};

MODULE_INIT(ModuleOperChans)
