/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "UbuntuMono Nerd Font:size=7", "JoyPixels:pixelsize=7:antialias=true:autohint=true"};
static const char dmenufont[]       = "monospace:size=7";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_red[]         = "#ff4545";
static const char col1[]            = "#ffffff";
static const char col2[]            = "#ffffff";
static const char col3[]            = "#ffffff";
static const char col4[]            = "#ffffff";
static const char col5[]            = "#ffffff";
static const char col6[]            = "#ffffff";
static const char col7[]            = "#ffffff";
static const char col8[]            = "#ffffff";
static const char col9[]            = "#ffffff";
static const char col10[]           = "#ffffff";
static const char col11[]           = "#ffffff";
static const char col12[]           = "#ffffff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]  = { col_gray3, col_gray1, col_gray2 },
	[SchemeCol1]  = { col1,      col_cyan,  col_gray2 },
	[SchemeCol2]  = { col_gray3, col_cyan,  col_gray2 },
	[SchemeCol3]  = { col_gray4, col_cyan,  col_gray2 },
	[SchemeCol4]  = { col4,      col_cyan,  col_cyan  },
	[SchemeCol5]  = { col5,      col_gray1, col_gray2 },
	[SchemeCol6]  = { col6,      col_gray1, col_gray2 },
	[SchemeCol7]  = { col7,      col_gray1, col_gray2 },
	[SchemeCol8]  = { col8,      col_gray1, col_gray2 },
	[SchemeCol9]  = { col8,      col_gray1, col_gray2 },
	[SchemeCol10] = { col10,     col_gray1, col_gray2 },
	[SchemeCol11] = { col11,     col_gray1, col_gray2 },
	[SchemeCol12] = { col12,     col_gray1, col_gray2 },
	[SchemeSel]   = { col_gray4, col_cyan,  col_red   },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title             tags mask     isfloating   isterminal noswallow monitor */
	{ "Gimp",     NULL,       NULL,             0,            1,           0,         0,        -1 },
	{ "Firefox",  NULL,       NULL,             1 << 8,       0,           0,         0,        -1 },
	{ "st",       NULL,       NULL,             0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           1,         1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static int isswallow = 1;
static int attachdirection = 2;    /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, /* change tag  */             \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, /* add tag */                 \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, /* move active win to tag  */ \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };

static const char *upbri[]   = { "/usr/bin/xbacklight", "-inc", "10", NULL };
static const char *downbri[] = { "/usr/bin/xbacklight", "-dec", "10", NULL };
static const char *scroff[] = { "/usr/bin/xset", "dpms", "force", "off", NULL }; // turn off screen

static const char *scrotcmd[]  = { "scrot", NULL };
static const char *scrotfocusedcmd[]  = { "scrot", "--focused", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } }, /* change active windows in stack  */
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} }, /* change width of master window  */
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} }, /* change master with selcted slave or next slave in stack  */
	{ MODKEY,                       XK_Tab,    view,           {0} }, /* alt-tab between 2 last tags  */
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} }, /* Terminate the process running in the current window  */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      spawn,	       SHCMD("killall xinit") },
	{ MODKEY|ControlMask,           XK_space,  focusmaster,    {0} },
	{ MODKEY|ControlMask,           XK_j,      pushdown,       {0} },
	{ MODKEY|ControlMask,           XK_k,      pushup,         {0} },
	{ MODKEY,			            XK_w,	   spawn,	       SHCMD("$BROWSER") },
	{ MODKEY,			            XK_v,	   spawn,	       SHCMD("clipmenu") },
    { 0,              XF86XK_AudioLowerVolume, spawn,          {.v = downvol } },
    { 0,                     XF86XK_AudioMute, spawn,          {.v = mutevol } },
    { 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = upvol   } },
    { MODKEY,                     XK_equal ,   spawn,          {.v = upbri   } },
    { MODKEY,                     XK_minus,    spawn,          {.v = downbri } },
	{ 0,    	       XF86XK_MonBrightnessUp, spawn,          {.v = upbri   } },
    { 0,  	    	 XF86XK_MonBrightnessDown, spawn,          {.v = downbri } },
	{ 0,  	    	       XF86XK_ScreenSaver, spawn,          {.v = scroff  } }, // turn off screen
    { MODKEY,                     XK_Down,     spawn,          {.v = downvol } },
    { MODKEY,                     XK_F9,       spawn,          {.v = mutevol } },
    { MODKEY,                     XK_Up,       spawn,          {.v = upvol   } },
	{ MODKEY|ControlMask,         XK_s,        toggleswallow,  {.i = 2       } }, /* arg > 1 -toogle, 0 or 1 - set arg */
	{ MODKEY|ControlMask,         XK_d,        nextdirection,  {.i = 6       } }, /* arg > 5 -toogle, 0 or 5 - set arg | 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
	{ 0,            			  XK_Print,    spawn,          {.v = scrotcmd } }, /* take screenshot by scrot*/
	{ ShiftMask,    			  XK_Print,    spawn,          {.v = scrotfocusedcmd } }, /* take screenshot of focused */
	{ ControlMask,  			  XK_Print,    spawn,           SHCMD("scrot --select --freeze") }, /* select aria for screnshot */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} }, // btn1 - left btn
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} }, // btn2 - middle btn (Cmd+Click)
    { ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} }, // btn3 - right btn
    { ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} }, // btn4 - scroll up   - vol up
    { ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} }, // btn5 - scroll down - vol down
    { ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
    { ClkStatusText,        ShiftMask,      Button3,        sigdwmblocks,   {.i = 7} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

