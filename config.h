/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
#if (HOST == macarch)
        "UbuntuMono Nerd Font:pixelsize=20:antialias=true:autohint=true",
        "UbuntuMono Nerd Font:size=10",
        "JoyPixels:pixelsize=10:antialias=true:autohint=true"
#else
        "UbuntuMono Nerd Font:pixelsize=20:antialias=true:autohint=true",
        "UbuntuMono Nerd Font:size=7",
        "JoyPixels:pixelsize=7:antialias=true:autohint=true"
#endif
};
#if (HOST == macarch)
static const char dmenufont[]       = "monospace:size=10";
#else
static const char dmenufont[]       = "monospace:size=7";
#endif
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
	[SchemeCol1]  = {  col_gray3,      col_gray1,  col_gray2 },  /* setxroot colors  */
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
	/* class          instance     title          tags mask     isfloating  isterminal  noswallow  monitor ignoretransient */
	{ "Gimp",           NULL,       NULL,             0,            0,           0,         0,        -1,       0},
	{ "jetbrains-idea", NULL,       NULL,             0,            0,           0,         0,        -1,       1},
	{ NULL,             NULL,       "webcam",         0,            1,           0,         0,        -1,       0},
	{ "Firefox",        NULL,       NULL,             1 << 8,       0,           0,         0,        -1,       0},
	{ "st",             NULL,       NULL,             0,            0,           1,         0,        -1,       0},
	{ NULL,             NULL,       "Event Tester",   0,            0,           1,         1,        -1,       0},
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static int isswallow = 1;
static int attachdirection = 2;      /* 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */

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
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, /* toggle tag to visible tags */                 \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, /* move active win to tag  */ \
	{ MODKEY|Mod1Mask,  			KEY,      toggletag,      {.ui = 1 << TAG} }, /* toggle win on tag */		\

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
#define UP_VOLUME  "/usr/bin/pactl set-sink-volume $(pacmd list-sinks | grep '\\* index:' | awk '{print $3}') +5%"
#define DOWN_VOLUME "/usr/bin/pactl set-sink-volume $(pacmd list-sinks | grep '\\* index:' | awk '{print $3}') -5%"
#define WEB_CAM "mpv --geometry=-0-0 --autofit=30% --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(\\ls /dev/video[0,2,4,6,8] | tail -n 1)"
static const char *mutevol[] = { "/usr/bin/pamixer", "--toggle-mute", NULL };

#if (HOST == macarch)
static const char *upbri[]   = { "gmux_backlight", "+100", NULL };
static const char *downbri[] = { "gmux_backlight", "-100", NULL };
#else
static const char *upbri[]   = { "/usr/bin/xbacklight", "-inc", "10", NULL };
static const char *downbri[] = { "/usr/bin/xbacklight", "-dec", "10", NULL };
#endif
static const char *scroff[] = { "/usr/bin/xset", "dpms", "force", "off", NULL }; // turn off screen
static const char *upkbdbri[]   = { "sudo", "macbook-lighter-kbd", "--inc", "17", NULL };
static const char *downkbdbri[] = { "sudo", "macbook-lighter-kbd", "--dec", "17", NULL };

static const char *scrotcmd[]  = { "scrot", NULL };
static const char *scrotfocusedcmd[]  = { "scrot", "--focused", NULL };

static const char *modc[] = { "/bin/bash", "copypaste.sh", "copy",  NULL };
static const char *modv[] = { "/bin/bash", "copypaste.sh", "paste", NULL };
// 										   xvkbd -xsendevent -text '\[Control_L]a'
static const char *moda[] = { "xvkbd", "-xsendevent", "-text", "\\[Control_L]a",  NULL };
static const char *modx[] = { "xvkbd", "-xsendevent", "-text", "\\[Control_L]x",  NULL };
static const char *modz[] = { "xvkbd", "-xsendevent", "-text", "\\[Control_L]z",  NULL };
static const char *mody[] = { "xvkbd", "-xsendevent", "-text", "\\[Control_L]y",  NULL };
static const char *mods[] = { "xvkbd", "-xsendevent", "-text", "\\[Control_L]s",  NULL };
static const char *modf[] = { "xvkbd", "-xsendevent", "-text", "\\[Control_L]f",  NULL };
static const char *modw[] = { "xvkbd", "-xsendevent", "-text", "\\[Control_L]w",  NULL };
static const char *modt[] = { "xvkbd", "-xsendevent", "-text", "\\[Control_L]t",  NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },		 /* open dmenu to run program */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },		 /* new st */
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} }, 					 /* Terminate the process running in the current window  */
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },					 /* relaunch dwm */

	{ MODKEY|ShiftMask,             XK_q,      spawn,	       SHCMD("killall xinit") }, /* exit X11 */
	{ MODKEY|ControlMask,           XK_b,      togglebar,      {0} },				     /* toggle bar */
	{ MODKEY|ControlMask,           XK_Return, togglefullscr,  {0} },				     /* toggle fullscreen */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } }, 		     /* go to (select, make active) next window in stack */
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },			 /* go to (select, make active) pre  window in stack */
	{ MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },					 /* move slave down in stack */
	{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },					 /* move slave up in stack */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} }, 			 /* change width of master window  - */
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },			 /* change width of master window  + */
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },				     /* toggle win from float to tiles and vise versa (dot in title bar means win is float)*/
	{ MODKEY,           			XK_space,  focusmaster,    {0} },			 		 /* go to (select) master */

	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },			 /* add win to   master aria */
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },			 /* del win from master aria */
	{ MODKEY,                       XK_Return, zoom,           {0} }, 				/* toggle a window between the master and stack area  */
	{ MODKEY,                       XK_Tab,    view,           {0} }, 				/* alt-tab between 2 last views  */

	{ MODKEY|ControlMask,           XK_t,      setlayout,      {.v = &layouts[0]} },/* tile, def layout */
	{ MODKEY|ControlMask,           XK_f,      setlayout,      {.v = &layouts[1]} },/* floating - no layout */
	{ MODKEY|ControlMask,           XK_m,      setlayout,      {.v = &layouts[2]} },/* monocle - 1 win per view, (mod+j/k - next win) */
	{ MODKEY|ControlMask,           XK_g,      setlayout,      {.v = &layouts[3]} },/* grid HH */
	{ MODKEY|ControlMask,           XK_u,      setlayout,      {.v = &layouts[4]} },/* master in center || || */
	{ MODKEY|ControlMask,           XK_o,      setlayout,      {.v = &layouts[5]} },/* floating master in center (why need it?) */
	{ MODKEY|ControlMask,           XK_space,  setlayout,      {0} }, /* toggle float and tile layout */

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },		/* select all views (show windows from all views) */
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },		/* make window visible in all views */

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

	{ MODKEY,			            XK_b,	   spawn,	       SHCMD("$BROWSER") }, // launch browser
    { 0,              XF86XK_AudioLowerVolume, spawn,          SHCMD(DOWN_VOLUME) },// vol down
    { 0,                     XF86XK_AudioMute, spawn,          {.v = mutevol } }, 	// vol mute
    { 0,              XF86XK_AudioRaiseVolume, spawn,          SHCMD(UP_VOLUME) }, 	// vol up
    { MODKEY,                     XK_Down,     spawn,          SHCMD(DOWN_VOLUME) },// vol down
    { MODKEY,                     XK_F9,       spawn,          {.v = mutevol } }, 	// vol mute
    { MODKEY,                     XK_Up,       spawn,          SHCMD(UP_VOLUME) }, 	// vol up
	{ MODKEY,                     XK_equal ,   spawn,          {.v = upbri   } }, 	// brightness up
    { MODKEY,                     XK_minus,    spawn,          {.v = downbri } }, 	// brightness down
	{ 0,    	       XF86XK_MonBrightnessUp, spawn,          {.v = upbri   } }, 	// brightness up
	{ 0,    	     XF86XK_KbdBrightnessUp,   spawn,          {.v = upkbdbri  } }, // brightness up
	{ 0,    	     XF86XK_KbdBrightnessDown, spawn,          {.v = downkbdbri} }, // brightness up
    { 0,  	    	 XF86XK_MonBrightnessDown, spawn,          {.v = downbri } }, 	// brightness down
    { 0,                   XF86XK_ScreenSaver, spawn,          {.v = scroff  } },   // turn off screen
	{ 0,  	    	           XF86XK_LaunchB, spawn,          {.v = scroff  } }, 	// turn off screen
	{ MODKEY|ControlMask,         XK_s,        toggleswallow,  {.i = 2       } }, 	/* arg > 1 = toogle, 0 or 1 - set arg */
	{ MODKEY|ControlMask,         XK_d,        nextdirection,  {.i = 6       } }, 	/* arg > 5 = toogle, 0 or 5 - set arg | 0 default, 1 above, 2 aside, 3 below, 4 bottom, 5 top */
    { 0,                          XK_Print,    spawn,          {.v = scrotcmd } },          /* take screenshot by scrot*/
    { ShiftMask,                  XK_Print,    spawn,          {.v = scrotfocusedcmd } },   /* take screenshot of focused */
    { ControlMask,                XK_Print,    spawn,           SHCMD("scrot --select --freeze") }, /* select aria for screnshot */
	{ 0,            		   XF86XK_LaunchA, spawn,          {.v = scrotcmd } }, 			/* take screenshot by scrot*/
	{ ShiftMask,    		   XF86XK_LaunchA, spawn,          {.v = scrotfocusedcmd } }, 	/* take screenshot of focused */
	{ ControlMask,  		   XF86XK_LaunchA, spawn,          SHCMD("scrot --select --freeze") }, /* select aria for screnshot */
	{ MODKEY,           		  XK_Right,    shiftview,  	   { .i = +1 } },  		// next view
	{ MODKEY,           		  XK_Left,     shiftview,      { .i = -1 } },  		// prev view
	{ MODKEY|ShiftMask,			  XK_v,	       spawn,	       SHCMD("clipmenu") }, // open clipboard history
    { MODKEY,                   XK_F11,         spawn,         SHCMD(WEB_CAM) },    // open mpv on /dev/video0 (web cam)
	{ MODKEY,                     XK_c,        spawn,          {.v = modc } }, 		// win+c = copy to cb
	{ MODKEY,                     XK_v,        spawn,          {.v = modv } }, 		// win+v = paste from cb
	{ MODKEY,                     XK_a,        spawn,          {.v = moda } }, 		// win+a = select all
	{ MODKEY,                     XK_x,        spawn,          {.v = modx } }, 		// win+x = cut to cb
	{ MODKEY,                     XK_z,        spawn,          {.v = modz } }, 		// win+z = undo
	{ MODKEY,                     XK_y,        spawn,          {.v = mody } }, 		// win+y = redo
	{ MODKEY,                     XK_s,        spawn,          {.v = mods } }, 		// win+s = save
	{ MODKEY,                     XK_f,        spawn,          {.v = modf } }, 		// win+f = find
	{ MODKEY,                     XK_w,        spawn,          {.v = modw } }, 		// win+w = close tab
	{ MODKEY,                     XK_t,        spawn,          {.v = modt } }, 		// win+t = new tab
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

