/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"

/* appearance */
static unsigned int borderpx  = 5;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad      = 10;       /* vertical padding of bar */
static const int sidepad      = 10;       /* horizontal padding of bar */
static const int user_bh      = 25;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will use user_bh as bar height */
static const int nrg_force_vsplit   = 1;
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const int usealtbar          = 0;        /* 1 means use non-dwm status bar */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const char *altbarcmd        = ""; /* Alternate bar launch command */
static const char *fonts[]          = { "Source Code Pro:size=10", "SauceCodePro Nerd Font:size=14" };
static const char dmenufont[]       = "SauceCodePro Nerd Font:size=10";
static const unsigned int baralpha = OPAQUE;
static const unsigned int borderalpha = OPAQUE;
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const char *alttags[] = { "", "", "", "", "", "", "", "", "" };

static const char scratchpadname[] = "scratchpad";
static const char btmscratchname[] = "btmscratch";
static const char kpscratchname[]  = "keepassxc";
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = { "st", "-n", scratchpadname, "-g", "120x34", NULL };
const char *spcmd2[] = { "st", "-n", btmscratchname, "-g", "240x68", "-e", "btm", NULL };
const char *spcmd3[] = { "keepassxc", NULL };
static Sp scratchpads[] = {
	/* name           cmd */
	{ scratchpadname, spcmd1 },
	{ btmscratchname, spcmd2 },
	{ kpscratchname,  spcmd3 },
};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance        title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,           NULL,           0,         1,          0,           0,        -1 },
	{ "St",      NULL,           NULL,           0,         0,          1,           0,        -1 },
	{ "Thunderbird",NULL,        NULL,           1 << 7,    0,          0,           0,        -1 },
	{ "Peek",    NULL,           NULL,           0,         1,          0,           0,        -1 },
	{ NULL,      NULL,           "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
	{ NULL,      NULL,           "pulsemixer",   0,         1,          0,           1,        -1 },
	{ NULL,      scratchpadname, NULL,           SPTAG(0),  1,          1,           1,        -1 },
	{ NULL,      btmscratchname, NULL,           SPTAG(1),  1,          1,           1,        -1 },
	{ NULL,      kpscratchname,  NULL,           SPTAG(2),  1,          0,           1,        -1 },
};

/* layout(s) */
static float mfact     = 0.65; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1 /*nrowgrid layout: foce two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "﬿",      tile },    /* first entry is default */
	{ "",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "頻",      centeredmaster },
	{ "恵",      centeredfloatingmaster },
	{ "H[]",      deck },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTMODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          	INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          SHCMD("$HOME/.config/rofi/launcher.sh") },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_grave,  togglescratch,  { .ui = 0 } },
	{ MODKEY,                       XK_apostrophe,togglescratch,{ .ui = 1 } },
	{ MODKEY,                       XK_p,      togglescratch,   { .ui = 2 } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f = 0} },
	{ MODKEY|Mod1Mask,              XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_a,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_a,      defaultgaps,    {0} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} }, /* floating */
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} }, /* monocle */
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[4]} }, /* centeredfloatingmaster */
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[5]} }, /* deck */
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	/* ====== Application shortcuts ====== */
	{ MODKEY,                       XK_d,      spawn,          SHCMD("open_calen") },
	{ MODKEY,                       XK_e,      spawn,          SHCMD("emacsclient -nc") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD(TERMINAL " -t pulsemixer -g 120x34 -e pulsemixer") },
	{ MODKEY,                       XK_r,      spawn,          SHCMD(TERMINAL " -e ranger") },
	{ MODKEY,                       XK_w,      spawn,          SHCMD("$BROWSER") },
	{ MODKEY,                       XK_semicolon,spawn,        SHCMD("rofi -show emoji -modi emoji") },
	{ 0,                            XK_Print,  spawn,          SHCMD("maimpick") },
	{ ShiftMask,                    XK_Print,  spawn,          SHCMD("peek") },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_Escape,   spawn,        SHCMD("power_menu") },
	{ MODKEY|ShiftMask,             XK_BackSpace, quit,        {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

