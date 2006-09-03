/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  InspIRCd is copyright (C) 2002-2006 ChatSpike-Dev.
 *                       E-mail:
 *                <brain@chatspike.net>
 *                <Craig@chatspike.net>
 *
 * Written by Craig Edwards, Craig McLure, and others.
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

#include "configreader.h"
#include "users.h"
#include "modules.h"
#include "commands/cmd_away.h"



extern "C" command_t* init_command(InspIRCd* Instance)
{
	return new cmd_away(Instance);
}

void cmd_away::Handle (const char** parameters, int pcnt, userrec *user)
{
	if ((pcnt) && (*parameters[0]))
	{
		strlcpy(user->awaymsg,parameters[0],MAXAWAY);
		user->WriteServ("306 %s :You have been marked as being away",user->nick);
		FOREACH_MOD(I_OnSetAway,OnSetAway(user));
	}
	else
	{
		*user->awaymsg = 0;
		user->WriteServ("305 %s :You are no longer marked as being away",user->nick);
		FOREACH_MOD(I_OnCancelAway,OnCancelAway(user));
	}
}
