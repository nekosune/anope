/* Ultimate IRCD 3 functions
 *
 * (C) 2003 Anope Team
 * Contact us at info@anope.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Epona by Lara.
 * Based on the original code of Services by Andy Church. 
 * 
 *
 */

#include "services.h"
#include "pseudo.h"

#ifdef IRC_ULTIMATE3

const char version_protocol[] = "UltimateIRCd 3.0.0.a26+";

IRCDVar ircd[] = {
    {"UltimateIRCd 3.0.*",      /* ircd name */
     "+S",                      /* nickserv mode */
     "+S",                      /* chanserv mode */
     "+S",                      /* memoserv mode */
     "+o",                      /* hostserv mode */
     "+iS",                     /* operserv mode */
     "+S",                      /* botserv mode  */
     "+Sh",                     /* helpserv mode */
     "+iS",                     /* Dev/Null mode */
     "+iS",                     /* Global mode   */
     "+o",                      /* nickserv alias mode */
     "+o",                      /* chanserv alias mode */
     "+o",                      /* memoserv alias mode */
     "+io",                     /* hostserv alias mode */
     "+io",                     /* operserv alias mode */
     "+o",                      /* botserv alias mode  */
     "+h",                      /* helpserv alias mode */
     "+i",                      /* Dev/Null alias mode */
     "+io",                     /* Global alias mode   */
     "+S",                      /* Used by BotServ Bots */
     5,                         /* Chan Max Symbols     */
     "-iklmnpqstRKAO",          /* Modes to Remove */
     "+ao",                     /* Channel Umode used by Botserv bots */
     1,                         /* SVSNICK */
     1,                         /* Vhost  */
     0,                         /* Has Owner */
     NULL,                      /* Mode to set for an owner */
     NULL,                      /* Mode to unset for an owner */
     "+rd",                     /* Mode On Reg          */
     "-r+d",                    /* Mode on UnReg        */
     "+d",                      /* Mode on Nick Change  */
     1,                         /* Supports SGlines     */
     1,                         /* Supports SQlines     */
     1,                         /* Supports SZlines     */
     1,                         /* Supports Halfop +h   */
     3,                         /* Number of server args */
     0,                         /* Join 2 Set           */
     0,                         /* Join 2 Message       */
     1,                         /* Has exceptions +e    */
     0,                         /* TS Topic Forward     */
     0,                         /* TS Topci Backward    */
     UMODE_p,                   /* Protected Umode      */
     1,                         /* Has Admin            */
     0,                         /* Chan SQlines         */
     0,                         /* Quit on Kill         */
     1,                         /* SVSMODE unban        */
     0,                         /* Has Protect          */
     0,                         /* Reverse              */
     1,                         /* Chan Reg             */
     CMODE_r,                   /* Channel Mode         */
     0,                         /* vidents              */
     0,                         /* svshold              */
     1,                         /* time stamp on mode   */
     0,                         /* NICKIP               */
     1,                         /* UMODE                */
     0,                         /* O:LINE               */
     1,                         /* VHOST ON NICK        */
     0,                         /* Change RealName      */
     CHAN_HELP_ULTIMATE3,       /* ChanServ extra   */
     CMODE_K,                   /* No Knock             */
     CMODE_A,                   /* Admin Only           */
     DEFAULT_MLOCK,             /* Default MLOCK       */
     UMODE_x,                   /* Vhost Mode           */
     0,                         /* +f                   */
     0,                         /* +L                   */
     0,                         /* +f Mode                          */
     0,                         /* +L Mode                              */
     1,                         /* On nick change check if they could be identified */
     1,                         /* No Knock requires +i */
     NULL,                      /* CAPAB Chan Modes             */
     0,                         /* We support TOKENS */
     1,                         /* TOKENS are CASE inSensitive */
     1,                         /* validate - to the #:# standard */
     },
    {NULL}
};

IRCDCAPAB ircdcap[] = {
    {
     CAPAB_NOQUIT,              /* NOQUIT       */
     CAPAB_TSMODE,              /* TSMODE       */
     CAPAB_UNCONNECT,           /* UNCONNECT    */
     0,                         /* NICKIP       */
     CAPAB_NSJOIN,              /* SJOIN        */
     CAPAB_ZIP,                 /* ZIP          */
     CAPAB_BURST,               /* BURST        */
     CAPAB_TS5,                 /* TS5          */
     0,                         /* TS3          */
     CAPAB_DKEY,                /* DKEY         */
     0,                         /* PT4          */
     0,                         /* SCS          */
     0,                         /* QS           */
     0,                         /* UID          */
     0,                         /* KNOCK        */
     CAPAB_CLIENT,              /* CLIENT       */
     CAPAB_IPV6,                /* IPV6         */
     CAPAB_SSJ5,                /* SSJ5         */
     0,                         /* SN2          */
     0,                         /* TOKEN        */
     0,                         /* VHOST        */
     0,                         /* SSJ3         */
     0,                         /* NICK2        */
     0,                         /* UMODE2       */
     0,                         /* VL           */
     0,                         /* TLKEXT       */
     CAPAB_DODKEY,              /* DODKEY       */
     CAPAB_DOZIP,               /* DOZIP        */
     0, 0}
};

void anope_set_umode(User * user, int ac, char **av)
{
    int add = 1;                /* 1 if adding modes, 0 if deleting */
    char *modes = av[0];

    ac--;

    if (debug)
        alog("debug: Changing mode for %s to %s", user->nick, modes);

    while (*modes) {

        add ? (user->mode |= umodes[(int) *modes]) : (user->mode &=
                                                      ~umodes[(int)
                                                              *modes]);

        switch (*modes++) {
        case '+':
            add = 1;
            break;
        case '-':
            add = 0;
            break;
        case 'a':
            if (add && !is_services_oper(user)) {
                send_cmd(ServerName, "SVSMODE %s -a", user->nick);
                user->mode &= ~UMODE_a;
            }
            break;
        case 'P':
            if (add && !is_services_admin(user)) {
                send_cmd(ServerName, "SVSMODE %s -P", user->nick);
                user->mode &= ~UMODE_P;
            }
            break;
        case 'Z':
            if (add && !is_services_root(user)) {
                send_cmd(ServerName, "SVSMODE %s -Z", user->nick);
                user->mode &= ~UMODE_Z;
            }
            break;
        case 'd':
            if (ac == 0) {
                alog("user: umode +d with no parameter (?) for user %s",
                     user->nick);
                break;
            }

            ac--;
            av++;
            user->svid = strtoul(*av, NULL, 0);
            break;
        case 'o':
            if (add) {
                opcnt++;

                if (WallOper)
                    anope_cmd_global(s_OperServ,
                                     "\2%s\2 is now an IRC operator.",
                                     user->nick);
                display_news(user, NEWS_OPER);
                if (is_services_oper(user)) {
                    send_cmd(ServerName, "SVSMODE %s +a", user->nick);
                    user->mode |= UMODE_a;
                }

                if (is_services_admin(user)) {
                    send_cmd(ServerName, "SVSMODE %s +P", user->nick);
                    user->mode |= UMODE_P;
                }

                if (is_services_root(user)) {
                    send_cmd(ServerName, "SVSMODE %s +Z", user->nick);
                    user->mode |= UMODE_Z;
                }
            } else {
                opcnt--;
            }
            break;
        case 'r':
            if (add && !nick_identified(user)) {
                send_cmd(ServerName, "SVSMODE %s -r", user->nick);
                user->mode &= ~UMODE_r;
            }
            break;
        case 'x':
            update_host(user);
            break;
        }
    }
}

unsigned long umodes[128] = {
    0, 0, 0,                    /* Unused */
    0, 0, 0,                    /* Unused */
    0, 0, 0,                    /* Unused, Unused, Horzontal Tab */
    0, 0, 0,                    /* Line Feed, Unused, Unused */
    0, 0, 0,                    /* Carriage Return, Unused, Unused */
    0, 0, 0,                    /* Unused */
    0, 0, 0,                    /* Unused */
    0, 0, 0,                    /* Unused */
    0, 0, 0,                    /* Unused */
    0, 0, 0,                    /* Unused */
    0, 0, 0,                    /* Unused, Unused, Space */
    0, 0, 0,                    /* ! " #  */
    0, 0, 0,                    /* $ % &  */
    0, 0, 0,                    /* ! ( )  */
    0, 0, 0,                    /* * + ,  */
    0, 0, 0,                    /* - . /  */
    0, 0,                       /* 0 1 */
    0, 0,                       /* 2 3 */
    0, 0,                       /* 4 5 */
    0, 0,                       /* 6 7 */
    0, 0,                       /* 8 9 */
    0, 0,                       /* : ; */
    0, 0, 0,                    /* < = > */
    0, 0,                       /* ? @ */
    UMODE_A, 0, 0,              /* A B C */
    UMODE_D, 0, 0,              /* D E F */
    0, 0, 0,                    /* G H I */
    0, 0, 0,                    /* J K L */
    0, 0, UMODE_0,              /* M N O */
    UMODE_P, 0, 0,              /* P Q R */
    UMODE_S, 0, 0,              /* S T U */
    0, UMODE_W, 0,              /* V W X */
    0,                          /* Y */
    UMODE_Z,                    /* Z */
    0, 0, 0,                    /* [ \ ] */
    0, 0, 0,                    /* ^ _ ` */
    UMODE_a, 0, 0,              /* a b c */
    UMODE_d, 0, 0,              /* d e f */
    0, UMODE_h, UMODE_i,        /* g h i */
    0, 0, 0,                    /* j k l */
    0, 0, UMODE_o,              /* m n o */
    UMODE_p, 0, UMODE_r,        /* p q r */
    0, 0, 0,                    /* s t u */
    0, UMODE_w, UMODE_x,        /* v w x */
    0,                          /* y */
    0,                          /* z */
    0, 0, 0,                    /* { | } */
    0, 0                        /* ~ � */
};


char csmodes[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0,
    'a',                        /* (33) ! Channel Admins */
    0, 0, 0,
    'h',                        /* (37) % Channel halfops */
    0, 0, 0, 0,
    0,

    'v', 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    'o', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


CMMode cmmodes[128] = {
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL},
    {NULL},
    {add_ban, del_ban},
    {NULL},
    {NULL},
    {add_exception, del_exception},
    {NULL},
    {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL},
    {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}, {NULL}
};


CBMode cbmodes[128] = {
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0},
    {CMODE_A, CBM_NO_USER_MLOCK, NULL, NULL},
    {0},                        /* B */
    {0},                        /* C */
    {0},                        /* D */
    {0},                        /* E */
    {0},                        /* F */
    {0},                        /* G */
    {0},                        /* H */
    {0},                        /* I */
    {0},                        /* J */
    {CMODE_K, 0, NULL, NULL},
    {0},                        /* L */
    {CMODE_M},
    {CMODE_N, 0, NULL, NULL},
    {CMODE_O, CBM_NO_USER_MLOCK, NULL, NULL},
    {0},                        /* P */
    {0},                        /* Q */
    {CMODE_R, 0, NULL, NULL},   /* R */
    {CMODE_S, 0, NULL, NULL},
    {0},                        /* T */
    {0},                        /* U */
    {0},                        /* V */
    {0},                        /* W */
    {0},                        /* X */
    {0},                        /* Y */
    {0},                        /* Z */
    {0}, {0}, {0}, {0}, {0}, {0},
    {0},                        /* a */
    {0},                        /* b */
    {0},                        /* c */
    {0},                        /* d */
    {0},                        /* e */
    {0},                        /* f */
    {0},                        /* g */
    {0},                        /* h */
    {CMODE_i, 0, NULL, NULL},
    {0},                        /* j */
    {CMODE_k, 0, set_key, cs_set_key},
    {CMODE_l, CBM_MINUS_NO_ARG, set_limit, cs_set_limit},
    {CMODE_m, 0, NULL, NULL},
    {CMODE_n, 0, NULL, NULL},
    {0},                        /* o */
    {CMODE_p, 0, NULL, NULL},
    {CMODE_q, 0, NULL, NULL},
    {CMODE_r, CBM_NO_MLOCK, NULL, NULL},
    {CMODE_s, 0, NULL, NULL},
    {CMODE_t, 0, NULL, NULL},
    {0},
    {0},                        /* v */
    {0},                        /* w */
    {0},                        /* x */
    {0},                        /* y */
    {0},                        /* z */
    {0}, {0}, {0}, {0}
};

CBModeInfo cbmodeinfos[] = {
    {'c', CMODE_c, 0, NULL, NULL},
    {'i', CMODE_i, 0, NULL, NULL},
    {'k', CMODE_k, 0, get_key, cs_get_key},
    {'l', CMODE_l, CBM_MINUS_NO_ARG, get_limit, cs_get_limit},
    {'m', CMODE_m, 0, NULL, NULL},
    {'n', CMODE_n, 0, NULL, NULL},
    {'p', CMODE_p, 0, NULL, NULL},
    {'q', CMODE_q, 0, NULL, NULL},
    {'r', CMODE_r, 0, NULL, NULL},
    {'s', CMODE_s, 0, NULL, NULL},
    {'t', CMODE_t, 0, NULL, NULL},
    {'A', CMODE_A, 0, NULL, NULL},
    {'K', CMODE_K, 0, NULL, NULL},
    {'N', CMODE_N, 0, NULL, NULL},
    {'O', CMODE_O, 0, NULL, NULL},
    {'R', CMODE_R, 0, NULL, NULL},
    {'S', CMODE_S, 0, NULL, NULL},
    {0}
};

CUMode cumodes[128] = {
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
    {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},

    {0},

    {CUS_PROTECT, CUF_PROTECT_BOTSERV, check_valid_admin},
    {0},                        /* b */
    {0},                        /* c */
    {0},                        /* d */
    {0},                        /* e */
    {0},                        /* f */
    {0},                        /* g */
    {CUS_HALFOP, 0, check_valid_op},
    {0},                        /* i */
    {0},                        /* j */
    {0},                        /* k */
    {0},                        /* l */
    {0},                        /* m */
    {0},                        /* n */
    {CUS_OP, CUF_PROTECT_BOTSERV, check_valid_op},
    {0},                        /* p */
    {0},                        /* q */
    {0},                        /* r */
    {0},                        /* s */
    {0},                        /* t */
    {0},                        /* u */
    {CUS_VOICE, 0, NULL},
    {0},                        /* w */
    {0},                        /* x */
    {0},                        /* y */
    {0},                        /* z */
    {0}, {0}, {0}, {0}, {0}
};


/* SVSMODE -b */
void anope_cmd_unban(char *name, char *nick)
{
    send_cmd(ServerName, "SVSMODE %s -b %s", name, nick);
}

int anope_event_sjoin(char *source, int ac, char **av)
{
    do_sjoin(source, ac, av);
    return MOD_CONT;
}


/*
** NICK - new
**      source  = NULL
**	parv[0] = nickname
**      parv[1] = hopcount
**      parv[2] = timestamp
**      parv[3] = modes
**      parv[4] = username
**      parv[5] = hostname
**      parv[6] = server
**	parv[7] = servicestamp
**      parv[8] = IP
**	parv[9] = info
** NICK - change 
**      source  = oldnick
**	parv[0] = new nickname
**      parv[1] = hopcount
*/
int anope_event_nick(char *source, int ac, char **av)
{
    if (ac != 2) {
        User *user = do_nick(source, av[0], av[4], av[5], av[6], av[9],
                             strtoul(av[2], NULL, 10), strtoul(av[7], NULL,
                                                               0),
                             strtoul(av[8], NULL, 0), "*", NULL);
        if (user)
            anope_set_umode(user, 1, &av[3]);
    } else {
        do_nick(source, av[0], NULL, NULL, NULL, NULL,
                strtoul(av[1], NULL, 10), 0, 0, NULL, NULL);
    }
    return MOD_CONT;
}

int anope_event_sethost(char *source, int ac, char **av)
{
    User *u;

    if (ac != 2)
        return MOD_CONT;

    u = finduser(av[0]);
    if (!u) {
        if (debug)
            alog("user: SETHOST for nonexistent user %s", av[0]);
        return MOD_CONT;
    }

    change_user_host(u, av[1]);
    return MOD_CONT;
}

int anope_event_capab(char *source, int ac, char **av)
{
    capab_parse(ac, av);
    return MOD_CONT;
}

/*
** CLIENT
**      source  = NULL       
**	parv[0] = nickname   Trystan
**      parv[1] = hopcount   1
**      parv[2] = timestamp  1090083810
**      parv[3] = modes      +ix
**	parv[4] = modes ?    +
**      parv[5] = username   Trystan
**      parv[6] = hostname   c-24-2-101-227.client.comcast.net
**      parv[7] = vhost      3223f75b.2b32ee69.client.comcast.net
**	parv[8] = server     WhiteRose.No.Eu.Shadow-Realm.org
**      parv[9] = svid       0 
**	parv[10] = ip         402810339
** 	parv[11] = info      Dreams are answers to questions not yet asked
*/
int anope_event_client(char *source, int ac, char **av)
{
    if (ac != 2) {
        User *user = do_nick(source, av[0], av[5], av[6], av[8], av[11],
                             strtoul(av[2], NULL, 10), strtoul(av[9], NULL,
                                                               0),
                             strtoul(av[10], NULL, 0), av[7], NULL);
        if (user) {
            anope_set_umode(user, 1, &av[3]);
        }
    }
    return MOD_CONT;
}

/* *INDENT-OFF* */
void moduleAddIRCDMsgs(void) {
    Message *m;

    m = createMessage("401",       NULL); addCoreMessage(IRCD,m);
    m = createMessage("436",       anope_event_436); addCoreMessage(IRCD,m);
    m = createMessage("AWAY",      anope_event_away); addCoreMessage(IRCD,m);
    m = createMessage("INVITE",    anope_event_invite); addCoreMessage(IRCD,m);
    m = createMessage("JOIN",      anope_event_join); addCoreMessage(IRCD,m);
    m = createMessage("KICK",      anope_event_kick); addCoreMessage(IRCD,m);
    m = createMessage("KILL",      anope_event_kill); addCoreMessage(IRCD,m);
    m = createMessage("MODE",      anope_event_mode); addCoreMessage(IRCD,m);
    m = createMessage("MOTD",      anope_event_motd); addCoreMessage(IRCD,m);
    m = createMessage("NICK",      anope_event_nick); addCoreMessage(IRCD,m);
    m = createMessage("NOTICE",    anope_event_notice); addCoreMessage(IRCD,m);
    m = createMessage("PART",      anope_event_part); addCoreMessage(IRCD,m);
    m = createMessage("PASS",      anope_event_pass); addCoreMessage(IRCD,m);
    m = createMessage("PING",      anope_event_ping); addCoreMessage(IRCD,m);
    m = createMessage("PRIVMSG",   anope_event_privmsg); addCoreMessage(IRCD,m);
    m = createMessage("QUIT",      anope_event_quit); addCoreMessage(IRCD,m);
    m = createMessage("SERVER",    anope_event_server); addCoreMessage(IRCD,m);
    m = createMessage("SQUIT",     anope_event_squit); addCoreMessage(IRCD,m);
    m = createMessage("TOPIC",     anope_event_topic); addCoreMessage(IRCD,m);
    m = createMessage("USER",      NULL); addCoreMessage(IRCD,m);
    m = createMessage("WALLOPS",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("WHOIS",     anope_event_whois); addCoreMessage(IRCD,m);
    m = createMessage("AKILL",     NULL); addCoreMessage(IRCD,m);
    m = createMessage("GLOBOPS",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("GNOTICE",   anope_event_gnotice); addCoreMessage(IRCD,m);
    m = createMessage("GOPER",     NULL); addCoreMessage(IRCD,m);
    m = createMessage("RAKILL",    NULL); addCoreMessage(IRCD,m);
    m = createMessage("SILENCE",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("SVSKILL",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("SVSMODE",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("SVSNICK",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("SVSNOOP",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("SQLINE",    anope_event_sqline); addCoreMessage(IRCD,m);
    m = createMessage("UNSQLINE",  NULL); addCoreMessage(IRCD,m);
    m = createMessage("CAPAB", 	   anope_event_capab); addCoreMessage(IRCD,m);
    m = createMessage("CS",        anope_event_cs); addCoreMessage(IRCD,m);
    m = createMessage("HS",        anope_event_hs); addCoreMessage(IRCD,m);
    m = createMessage("MS",        anope_event_ms); addCoreMessage(IRCD,m);
    m = createMessage("NS",        anope_event_ns); addCoreMessage(IRCD,m);
    m = createMessage("OS",        anope_event_os); addCoreMessage(IRCD,m);
    m = createMessage("RS",        NULL); addCoreMessage(IRCD,m);
    m = createMessage("SGLINE",    NULL); addCoreMessage(IRCD,m);
    m = createMessage("SJOIN",     anope_event_sjoin); addCoreMessage(IRCD,m);
    m = createMessage("SS",        NULL); addCoreMessage(IRCD,m);
    m = createMessage("SVINFO",    anope_event_svinfo); addCoreMessage(IRCD,m);
    m = createMessage("SZLINE",    NULL); addCoreMessage(IRCD,m);
    m = createMessage("UNSGLINE",  NULL); addCoreMessage(IRCD,m);
    m = createMessage("UNSZLINE",  NULL); addCoreMessage(IRCD,m);
    m = createMessage("SETHOST",   anope_event_sethost); addCoreMessage(IRCD,m);
    m = createMessage("NETINFO",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("GCONNECT",  NULL); addCoreMessage(IRCD,m);
    m = createMessage("NETGLOBAL", anope_event_netglobal); addCoreMessage(IRCD,m);
    m = createMessage("CHATOPS",   NULL); addCoreMessage(IRCD,m);
    m = createMessage("NETCTRL",   anope_event_netctrl); addCoreMessage(IRCD,m);
    m = createMessage("CLIENT",	   anope_event_client); addCoreMessage(IRCD,m);
    m = createMessage("SMODE",	   NULL); addCoreMessage(IRCD,m);
    m = createMessage("ERROR",	   anope_event_error); addCoreMessage(IRCD,m);
    m = createMessage("BURST",	   anope_event_burst); addCoreMessage(IRCD,m);
    m = createMessage("REHASH",    anope_event_rehash); addCoreMessage(IRCD,m);
    m = createMessage("ADMIN",     anope_event_admin); addCoreMessage(IRCD,m);
    m = createMessage("CREDITS",   anope_event_credits); addCoreMessage(IRCD,m);
    m = createMessage("SMODE",     anope_event_mode); addCoreMessage(IRCD,m);
}

/* *INDENT-ON* */


void anope_cmd_sqline(char *mask, char *reason)
{
    if (!mask || !reason) {
        return;
    }

    send_cmd(NULL, "SQLINE %s :%s", mask, reason);
}

void anope_cmd_unsgline(char *mask)
{
    send_cmd(NULL, "UNSGLINE 0 :%s", mask);
}

void anope_cmd_unszline(char *mask)
{
    send_cmd(NULL, "UNSZLINE 0 %s", mask);
}
void anope_cmd_szline(char *mask, char *reason)
{
    send_cmd(NULL, "SZLINE %s :%s", mask, reason);
}

void anope_cmd_svsnoop(char *server, int set)
{
    send_cmd(NULL, "SVSNOOP %s %s", server, (set ? "+" : "-"));
}

void anope_cmd_svsadmin(char *server, int set)
{
    anope_cmd_svsnoop(server, set);
}

void anope_cmd_sgline(char *mask, char *reason)
{
    send_cmd(NULL, "SGLINE %d :%s:%s", strlen(mask), mask, reason);
}

void anope_cmd_remove_akill(char *user, char *host)
{
    send_cmd(NULL, "RAKILL %s %s", host, user);
}

void anope_cmd_vhost_off(char *nick)
{
    send_cmd(s_HostServ, "SVSMODE %s -x", nick);
}

void anope_cmd_vhost_on(char *nick, char *vIdent, char *vhost)
{
    send_cmd(s_HostServ, "SVSMODE %s +x", nick);
    send_cmd(ServerName, "SETHOST %s %s", nick, vhost);
}

void anope_cmd_join(char *user, char *channel, time_t chantime)
{
    send_cmd(user, "SJOIN %ld %s", chantime, channel);
}

void anope_cmd_akill(char *user, char *host, char *who, time_t when,
                     time_t expires, char *reason)
{
    send_cmd(NULL, "AKILL %s %s %d %s %ld :%s", host, user, 86400 * 2, who,
             time(NULL), reason);
}

void anope_cmd_svskill(char *source, char *user, const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(NULL, "SVSKILL %s :%s", user, buf);
}


void anope_cmd_svsmode(User * u, int ac, char **av)
{
    send_cmd(ServerName, "SVSMODE %s %ld %s%s%s", u->nick, u->timestamp,
             av[0], (ac == 2 ? " " : ""), (ac == 2 ? av[1] : ""));
}

void anope_squit(char *servname, char *message)
{
    send_cmd(servname, "SQUIT %s :%s", servname, message);
}

void anope_pong(char *servname)
{
    send_cmd(servname, "PONG %s", servname);
}

/* Events */

int anope_event_ping(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    anope_cmd_pong(ac > 1 ? av[1] : ServerName, av[0]);
    return MOD_CONT;
}

int anope_event_436(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;

    m_nickcoll(av[0]);
    return MOD_CONT;
}

int anope_event_away(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;

    if (!source) {
        return MOD_CONT;
    }
    m_away(source, av[0]);
    return MOD_CONT;
}

int anope_event_topic(char *source, int ac, char **av)
{
    if (ac != 4)
        return MOD_CONT;
    do_topic(source, ac, av);
    return MOD_CONT;
}

int anope_event_squit(char *source, int ac, char **av)
{
    if (ac != 2)
        return MOD_CONT;
    do_squit(source, ac, av);
    return MOD_CONT;
}

int anope_event_quit(char *source, int ac, char **av)
{
    if (ac != 1)
        return MOD_CONT;
    do_quit(source, ac, av);
    return MOD_CONT;
}


int anope_event_mode(char *source, int ac, char **av)
{
    if (ac < 2)
        return MOD_CONT;

    if (*av[0] == '#' || *av[0] == '&') {
        do_cmode(source, ac, av);
    } else {
        do_umode(source, ac, av);
    }
    return MOD_CONT;
}

/* EVENT : OS */
int anope_event_os(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_OperServ, av[0]);
    return MOD_CONT;
}

/* EVENT : NS */
int anope_event_ns(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_NickServ, av[0]);
    return MOD_CONT;
}

/* EVENT : MS */
int anope_event_ms(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_MemoServ, av[0]);
    return MOD_CONT;
}

/* EVENT : HS */
int anope_event_hs(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_HostServ, av[0]);
    return MOD_CONT;
}

/* EVENT : CS */
int anope_event_cs(char *source, int ac, char **av)
{
    if (ac < 1)
        return MOD_CONT;
    m_privmsg(source, s_ChanServ, av[0]);
    return MOD_CONT;
}


int anope_event_kill(char *source, int ac, char **av)
{
    if (ac != 2)
        return MOD_CONT;

    m_kill(av[0], av[1]);
    return MOD_CONT;
}

int anope_event_kick(char *source, int ac, char **av)
{
    if (ac != 3)
        return MOD_CONT;
    do_kick(source, ac, av);
    return MOD_CONT;
}


int anope_event_join(char *source, int ac, char **av)
{
    if (ac != 1)
        return MOD_CONT;
    do_join(source, ac, av);
    return MOD_CONT;
}

int anope_event_motd(char *source, int ac, char **av)
{
    if (!source) {
        return MOD_CONT;
    }

    m_motd(source);
    return MOD_CONT;
}

int anope_event_setname(char *source, int ac, char **av)
{
    User *u;

    if (ac != 1)
        return MOD_CONT;

    u = finduser(source);
    if (!u) {
        alog("user: SETNAME for nonexistent user %s", source);
        return MOD_CONT;
    }

    change_user_realname(u, av[0]);
    return MOD_CONT;
}

int anope_event_chgname(char *source, int ac, char **av)
{
    User *u;

    if (ac != 2)
        return MOD_CONT;

    u = finduser(av[0]);
    if (!u) {
        alog("user: CHGNAME for nonexistent user %s", av[0]);
        return MOD_CONT;
    }

    change_user_realname(u, av[1]);
    return MOD_CONT;
}

int anope_event_setident(char *source, int ac, char **av)
{
    User *u;

    if (ac != 1)
        return MOD_CONT;

    u = finduser(source);
    if (!u) {
        alog("user: SETIDENT for nonexistent user %s", source);
        return MOD_CONT;
    }

    change_user_username(u, av[0]);
    return MOD_CONT;
}
int anope_event_chgident(char *source, int ac, char **av)
{
    User *u;

    if (ac != 2)
        return MOD_CONT;

    u = finduser(av[0]);
    if (!u) {
        alog("user: CHGIDENT for nonexistent user %s", av[0]);
        return MOD_CONT;
    }

    change_user_username(u, av[1]);
    return MOD_CONT;
}

/* EVENT: SERVER */
int anope_event_server(char *source, int ac, char **av)
{
    char *uplink;

    if (!stricmp(av[1], "1"))
        uplink = sstrdup(av[0]);
    do_server(source, av[0], av[1], av[2], NULL);
    return MOD_CONT;
}


int anope_event_privmsg(char *source, int ac, char **av)
{
    if (ac != 2)
        return MOD_CONT;
    m_privmsg(source, av[0], av[1]);
    return MOD_CONT;
}

int anope_event_part(char *source, int ac, char **av)
{
    if (ac < 1 || ac > 2)
        return MOD_CONT;
    do_part(source, ac, av);
    return MOD_CONT;
}

int anope_event_whois(char *source, int ac, char **av)
{
    if (source && ac >= 1) {
        m_whois(source, av[0]);
    }
    return MOD_CONT;
}

void anope_cmd_topic(char *whosets, char *chan, char *whosetit,
                     char *topic, time_t when)
{
    send_cmd(whosets, "TOPIC %s %s %lu :%s", chan, whosetit, when, topic);
}

void anope_cmd_372(char *source, char *msg)
{
    send_cmd(ServerName, "372 %s :- %s", source, msg);
}

void anope_cmd_372_error(char *source)
{
    send_cmd(ServerName, "372 %s :- MOTD file not found!  Please "
             "contact your IRC administrator.", source);
}

void anope_cmd_375(char *source)
{
    send_cmd(ServerName, "375 %s :- %s Message of the Day",
             source, ServerName);
}

void anope_cmd_376(char *source)
{
    send_cmd(ServerName, "376 %s :End of /MOTD command.", source);
}

void anope_cmd_nick(char *nick, char *name, char *modes)
{
    EnforceQlinedNick(nick, NULL);
    send_cmd(NULL, "CLIENT %s 1 %ld %s + %s %s * %s 0 0 :%s", nick,
             time(NULL), modes, ServiceUser, ServiceHost, ServerName,
             name);
    anope_cmd_sqline(nick, "Reserved for services");
}

void anope_cmd_guest_nick(char *nick, char *user, char *host, char *real,
                          char *modes)
{
    send_cmd(NULL, "CLIENT %s 1 %ld %s + %s %s * %s 0 0 :%s", nick,
             time(NULL), modes, user, host, ServerName, real);
}

void anope_cmd_mode(char *source, char *dest, const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(source, "MODE %s %s", dest, buf);
}

void anope_cmd_bot_nick(char *nick, char *user, char *host, char *real,
                        char *modes)
{
    EnforceQlinedNick(nick, s_BotServ);
    send_cmd(NULL, "CLIENT %s 1 %ld %s + %s %s * %s 0 0 :%s", nick,
             time(NULL), modes, user, host, ServerName, real);
    anope_cmd_sqline(nick, "Reserved for services");
}

void anope_cmd_kick(char *source, char *chan, char *user, const char *fmt,
                    ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }

    if (buf) {
        send_cmd(source, "KICK %s %s :%s", chan, user, buf);
    } else {
        send_cmd(source, "KICK %s %s", chan, user);
    }
}

void anope_cmd_notice_ops(char *source, char *dest, const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(NULL, "NOTICE @%s :%s", dest, buf);
}


void anope_cmd_notice(char *source, char *dest, const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    if (UsePrivmsg) {
        anope_cmd_privmsg2(source, dest, buf);
    } else {
        send_cmd(source, "NOTICE %s :%s", dest, buf);
    }
}

void anope_cmd_notice2(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE %s :%s", dest, msg);
}

void anope_cmd_privmsg(char *source, char *dest, const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(source, "PRIVMSG %s :%s", dest, buf);
}

void anope_cmd_privmsg2(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG %s :%s", dest, msg);
}

void anope_cmd_serv_notice(char *source, char *dest, char *msg)
{
    send_cmd(source, "NOTICE $%s :%s", dest, msg);
}

void anope_cmd_serv_privmsg(char *source, char *dest, char *msg)
{
    send_cmd(source, "PRIVMSG $%s :%s", dest, msg);
}

void anope_cmd_bot_chan_mode(char *nick, char *chan)
{
    anope_cmd_mode(nick, chan, "%s %s %s", ircd->botchanumode, nick, nick);
}

void anope_cmd_351(char *source)
{
    send_cmd(ServerName, "351 %s Anope-%s %s :%s - %s -- %s",
             source, version_number, ServerName, ircd->name, version_flags,
             version_build);
}

/* QUIT */
void anope_cmd_quit(char *source, const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    if (buf) {
        send_cmd(source, "QUIT :%s", buf);
    } else {
        send_cmd(source, "QUIT");
    }
}

/* PROTOCTL */
void anope_cmd_capab()
{
    send_cmd(NULL,
             "CAPAB TS5 NOQUIT SSJ5 BURST UNCONNECT TSMODE NICKIP CLIENT");
}

/* PASS */
void anope_cmd_pass(char *pass)
{
    send_cmd(NULL, "PASS %s :TS", pass);
}

/* SERVER name hop descript */
/* Unreal 3.2 actually sends some info about itself in the descript area */
void anope_cmd_server(char *servname, int hop, char *descript)
{
    send_cmd(NULL, "SERVER %s %d :%s", servname, hop, descript);
}

/* PONG */
void anope_cmd_pong(char *servname, char *who)
{
    send_cmd(servname, "PONG %s", who);
}

/* UNSQLINE */
void anope_cmd_unsqline(char *user)
{
    if (!user) {
        return;
    }
    send_cmd(NULL, "UNSQLINE %s", user);
}

/* CHGHOST */
void anope_cmd_chghost(char *nick, char *vhost)
{
    if (!nick || !vhost) {
        return;
    }
    send_cmd(ServerName, "CHGHOST %s %s", nick, vhost);
}

/* CHGIDENT */
void anope_cmd_chgident(char *nick, char *vIdent)
{
    if (!nick || !vIdent) {
        return;
    }
    send_cmd(ServerName, "CHGIDENT %s %s", nick, vIdent);
}

/* INVITE */
void anope_cmd_invite(char *source, char *chan, char *nick)
{
    if (!source || !chan || !nick) {
        return;
    }

    send_cmd(source, "INVITE %s %s", nick, chan);
}

/* PART */
void anope_cmd_part(char *nick, char *chan, const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }

    if (!nick || !chan) {
        return;
    }

    if (buf) {
        send_cmd(nick, "PART %s :%s", chan, buf);
    } else {
        send_cmd(nick, "PART %s", chan);
    }
}

/* 391 */
void anope_cmd_391(char *source, char *timestr)
{
    if (!timestr) {
        return;
    }
    send_cmd(NULL, "391 :%s %s :%s", source, ServerName, timestr);
}

/* 250 */
void anope_cmd_250(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(NULL, "250 %s ", buf);
}

/* 307 */
void anope_cmd_307(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(ServerName, "307 %s ", buf);
}

/* 311 */
void anope_cmd_311(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(ServerName, "311 %s ", buf);
}

/* 312 */
void anope_cmd_312(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(ServerName, "312 %s ", buf);
}

/* 317 */
void anope_cmd_317(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(ServerName, "317 %s ", buf);
}

/* 219 */
void anope_cmd_219(char *source, char *letter)
{
    if (!source) {
        return;
    }

    if (letter) {
        send_cmd(NULL, "219 %s %c :End of /STATS report.", source,
                 *letter);
    } else {
        send_cmd(NULL, "219 %s l :End of /STATS report.", source);
    }
}

/* 401 */
void anope_cmd_401(char *source, char *who)
{
    if (!source || !who) {
        return;
    }
    send_cmd(ServerName, "401 %s %s :No such service.", source, who);
}

/* 318 */
void anope_cmd_318(char *source, char *who)
{
    if (!source || !who) {
        return;
    }

    send_cmd(ServerName, "318 %s %s :End of /WHOIS list.", source, who);
}

/* 242 */
void anope_cmd_242(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(NULL, "242 %s ", buf);
}

/* 243 */
void anope_cmd_243(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(NULL, "243 %s ", buf);
}

/* 211 */
void anope_cmd_211(const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(NULL, "211 %s ", buf);
}

/* GLOBOPS */
void anope_cmd_global(char *source, const char *fmt, ...)
{
    va_list args;
    char buf[BUFSIZE];
    *buf = '\0';

    if (fmt) {
        va_start(args, fmt);
        vsnprintf(buf, BUFSIZE - 1, fmt, args);
        va_end(args);
    }
    if (!buf) {
        return;
    }

    send_cmd(source ? source : ServerName, "GLOBOPS :%s", buf);
}

/* SQUIT */
void anope_cmd_squit(char *servname, char *message)
{
    if (!servname || !message) {
        return;
    }

    send_cmd(servname, "SQUIT %s :%s", servname, message);
}

/* SVSO */
void anope_cmd_svso(char *source, char *nick, char *flag)
{
    if (!source || !nick || !flag) {
        return;
    }

    send_cmd(source, "SVSO %s %s", nick, flag);
}

/* NICK <newnick>  */
void anope_cmd_chg_nick(char *oldnick, char *newnick)
{
    if (!oldnick || !newnick) {
        return;
    }

    send_cmd(oldnick, "NICK %s", newnick);
}

/* SVSNICK */
void anope_cmd_svsnick(char *source, char *guest, time_t when)
{
    if (!source || !guest) {
        return;
    }
    send_cmd(NULL, "SVSNICK %s %s :%ld", source, guest, when);
}

/* Functions that use serval cmd functions */

void anope_cmd_connect(int servernum)
{
    if (!servernum) {
        servernum = 1;
    }

    if (servernum == 1) {
        anope_cmd_pass(RemotePassword);
    }
    if (servernum == 2) {
        anope_cmd_pass(RemotePassword2);
    }
    if (servernum == 3) {
        anope_cmd_pass(RemotePassword3);
    }
    anope_cmd_capab();
    anope_cmd_server(ServerName, 1, ServerDesc);
    anope_cmd_svinfo();
    anope_cmd_burst();
}

/* SVSHOLD - set */
void anope_cmd_svshold(char *nick)
{
    /* Not supported by this IRCD */
}

/* SVSHOLD - release */
void anope_cmd_release_svshold(char *nick)
{
    /* Not Supported by this IRCD */
}

/* SVSMODE +d */
/* sent if svid is something weird */
void anope_cmd_svid_umode(char *nick, time_t ts)
{
    send_cmd(ServerName, "SVSMODE %s %lu +d 1", nick, ts);
}

/* SVSMODE +d */
/* nc_change was = 1, and there is no na->status */
void anope_cmd_nc_change(User * u)
{
    common_svsmode(u, "+d", "1");
}

/* SVSMODE +d */
void anope_cmd_svid_umode2(User * u, char *ts)
{
    // not used by bahamut ircds
}

void anope_cmd_svid_umode3(User * u, char *ts)
{
    if (u->svid != u->timestamp) {
        common_svsmode(u, "+rd", ts);
    } else {
        common_svsmode(u, "+r", NULL);
    }

}

int anope_event_svinfo(char *source, int ac, char **av)
{
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

/*
 * SVINFO 
 *       parv[0] = sender prefix 
 *       parv[1] = TS_CURRENT for the server 
 *       parv[2] = TS_MIN for the server 
 *       parv[3] = server is standalone or connected to non-TS only 
 *       parv[4] = server's idea of UTC time
 */
void anope_cmd_svinfo()
{
    send_cmd(NULL, "SVINFO 5 3 0 :%ld", time(NULL));
}

int anope_event_pass(char *source, int ac, char **av)
{
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

int anope_event_gnotice(char *source, int ac, char **av)
{
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

int anope_event_netctrl(char *source, int ac, char **av)
{
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

int anope_event_notice(char *source, int ac, char **av)
{
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

int anope_event_sqline(char *source, int ac, char **av)
{
    /* currently not used but removes the message : unknown message from server */
    return MOD_CONT;
}

void anope_cmd_svsjoin(char *source, char *nick, char *chan)
{
    /* Not Supported by this IRCD */
}

void anope_cmd_svspart(char *source, char *nick, char *chan)
{
    /* Not Supported by this IRCD */
}

void anope_cmd_swhois(char *source, char *who, char *mask)
{
    /* not supported */
}


void anope_cmd_eob()
{
    send_cmd(NULL, "BURST 0");
}

void anope_cmd_burst()
{
    send_cmd(NULL, "BURST");
}

int anope_event_error(char *source, int ac, char **av)
{
    if (ac >= 1) {
        if (debug) {
            alog("ERROR: %s", av[0]);
        }
    }
    return MOD_CONT;
}

int anope_event_burst(char *source, int ac, char **av)
{
    Server *s;
    s = findserver(servlist, source);
    if (!ac) {
        /* for future use  - start burst */
    } else {
        if (s) {
            s->sync = 1;
        }
    }
    return MOD_CONT;
}

int anope_event_rehash(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int anope_event_credits(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int anope_event_admin(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int anope_event_netglobal(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int anope_event_invite(char *source, int ac, char **av)
{
    return MOD_CONT;
}

int anope_flood_mode_check(char *value)
{
    return 0;
}


#endif
