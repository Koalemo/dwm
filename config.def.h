/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_purp[]        = "#9932cc";

static char *colors[][3]      = {
	/*               fg         bg         border   */
   	[SchemeRb0]  = { col_gray4, "#698EB3", col_purp},
   	[SchemeRb1]  = { col_gray4, "#216f90", col_purp},
   	[SchemeRb2]  = { col_gray4, "#489dc7", col_purp},
   	[SchemeRb3]  = { col_gray4, "#5ec2eb", col_purp},
   	[SchemeRb4]  = { col_gray4, "#3a89ef", col_purp},
   	[SchemeRb5]  = { col_gray4, "#175bb1", col_purp},
   	[SchemeRb6]  = { col_gray4, "#2d6271", col_purp},
   	[SchemeRb7]  = { col_gray4, "#bec3f2", col_purp},
   	[SchemeRb8]  = { col_gray4, "#c9eafd", col_purp},
   	[SchemeRb9]  = { col_gray4, "#7fc3db", col_purp},
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const int dirs[3]      = { DirHor, DirVer, DirVer }; /* tiling dirs */
static const float facts[3]   = { 1.1,    1.1,    1.1 };    /* tiling facts */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */
static int       attachbelowactive = 1;    /* 1 means attach after curently active window*/

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define TILEKEYS(MOD,G,M,S) \
	{ MOD, XK_r, setdirs,  {.v = (int[])  { INC(G * +1),   INC(M * +1),   INC(S * +1) } } }, \
	{ MOD, XK_h, setfacts, {.v = (float[]){ INC(G * -0.1), INC(M * -0.1), INC(S * -0.1) } } }, \
	{ MOD , XK_l, setfacts, {.v = (float[]){ INC(G * +0.1), INC(M * +0.1), INC(S * +0.1) } } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *browsercmd[] = {"qutebrowser"};

static const Key keys[] = {
	/* modifier                     key        function               argument */
	{ MODKEY,                       XK_space,  spawn,                 {.v = dmenucmd } }, // open dmenu
	{ MODKEY|ShiftMask,             XK_Return, spawn,                 {.v = termcmd } },  // open new terminal window
	{ MODKEY,                       XK_w,      spawn,                 {.v = browsercmd } },  // open new browser windwow
	{ MODKEY,                       XK_b,      togglebar,             {0} },
	{ MODKEY,                       XK_j,      focusstack,            {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,            {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      inplacerotate,  {.i = +3} }, // rotate masters
	{ MODKEY|ShiftMask,             XK_k,      inplacerotate,  {.i = -3} }, // rotate masters
	{ MODKEY|ControlMask,           XK_j,      inplacerotate,  {.i = +4} }, // rotate stack
	{ MODKEY|ControlMask,           XK_k,      inplacerotate,  {.i = -4} }, // rotate stack
  { MODKEY,                       XK_period, switchcol,   {0} },
 	{ MODKEY,                       XK_n,      movestack,      {.i = +1 } },
 	{ MODKEY,                       XK_p,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,            {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,            {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,                  {0} },
	{ MODKEY,                       XK_Tab,    view,                  {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,            {0} },
	{ MODKEY,                       XK_t,      setlayout,             {.v = &layouts[0]} }, // monocle
	{ MODKEY,                       XK_f,      setlayout,             {.v = &layouts[1]} }, // floating
	{ MODKEY,                       XK_m,      setlayout,             {.v = &layouts[2]} }, // monocle

	{ MODKEY | ShiftMask,          XK_space,  setlayout,              {0} },
 	TILEKEYS(MODKEY,                                           1, 0, 0)
 	TILEKEYS(MODKEY|ShiftMask,                                 0, 1, 0)
 	TILEKEYS(MODKEY|ControlMask,                               0, 0, 1)
 	TILEKEYS(MODKEY|ShiftMask|ControlMask,                     1, 1, 1)
 	{ MODKEY|ShiftMask,             XK_t,      swapdirs,       {0} },
// { MODKEY|ShiftMask,             XK_space,  togglefloating,         {0} },
	{ MODKEY,                       XK_0,      view,                  {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,                   {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,              {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,              {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,                {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,                {.i = +1 } },
	TAGKEYS(                        XK_1,                             0)
	TAGKEYS(                        XK_2,                             1)
	TAGKEYS(                        XK_3,                             2)
	TAGKEYS(                        XK_4,                             3)
	TAGKEYS(                        XK_5,                             4)
	TAGKEYS(                        XK_6,                             5)
	TAGKEYS(                        XK_7,                             6)
	TAGKEYS(                        XK_8,                             7)
	TAGKEYS(                        XK_9,                             8)
        { MODKEY|Mod1Mask,              XK_1,      focusbynum,            {.i = 0} }, // Focus window within tag
 	{ MODKEY|Mod1Mask,              XK_2,      focusbynum,            {.i = 1} }, // Focus window within tag
 	{ MODKEY|Mod1Mask,              XK_3,      focusbynum,            {.i = 2} }, // Focus window within tag
 	{ MODKEY|Mod1Mask,              XK_4,      focusbynum,            {.i = 3} }, // Focus window within tag
 	{ MODKEY|Mod1Mask,              XK_5,      focusbynum,            {.i = 4} }, // Focus window within tag
 	{ MODKEY|Mod1Mask,              XK_6,      focusbynum,            {.i = 5} }, // Focus window within tag
 	{ MODKEY|Mod1Mask,              XK_7,      focusbynum,            {.i = 6} }, // Focus window within tag
 	{ MODKEY|Mod1Mask,              XK_8,      focusbynum,            {.i = 7} }, // Focus window within tag
 	{ MODKEY|Mod1Mask,              XK_9,      focusbynum,            {.i = 8} }, // Focus window within tag
	{ MODKEY|ShiftMask,             XK_q,      quit,                  {1} },      // restart 
	{ MODKEY|ShiftMask|ControlMask, XK_q,      quit,                  {0} },      // quit
	{ MODKEY|ShiftMask,             XK_b,      toggleattachbelow,     {0} },
  { MODKEY,                       XK_s,      togglesticky,          {0} },
	// TODO: fix hardcoded paths
	{ 0,				                    XF86XK_MonBrightnessUp,   spawn,     SHCMD("~/workspaceconfig/DEPS/scripts/shell/backlight --inc 1") },
	{ 0,				                    XF86XK_MonBrightnessDown, spawn,     SHCMD("~/workspaceconfig/DEPS/scripts/shell/backlight --dec 3") },
	{ 0,				                    XF86XK_AudioRaiseVolume,  spawn,     SHCMD("~/workspaceconfig/DEPS/scripts/shell/volume --inc 1 ") },
	{ 0,				                    XF86XK_AudioLowerVolume,  spawn,     SHCMD("~/workspaceconfig/DEPS/scripts/shell/volume --dec 1") },
	{ 0,				                    XF86XK_AudioMute,         spawn,     SHCMD("~/workspaceconfig/DEPS/scripts/shell/volume --mute") },
	{ MODKEY|ShiftMask,				      XK_space,                 spawn,     SHCMD("~/workspaceconfig/DEPS/scripts/shortcut-menus/keyboard-layout") },
};
//static const char *inclight[] = {"~/code/scripts/shell/backlight --inc 5"};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

