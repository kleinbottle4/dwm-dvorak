/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 0;       /* snap pixel */
static const unsigned int minwsz    = 20;       /* Minimal heigt of a client for smfact */
static const unsigned int cornerrad = 10;
static const unsigned int gappih    = 5;
static const unsigned int gappiv    = 5;
static const unsigned int gappoh    = 5;
static const unsigned int gappov    = 5;
static const int smartgaps          = 0;
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char dmenufont[]       = "Inconsolata:size=20";
static const char *fonts[]          = { dmenufont };
static const char col_nb[]       = "black";
static const char col_nbo[]       = "white";
static const char col_nf[]       = "white";
static const char col_sf[]       = "white";
static const char col_sb[]        = "grey10";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_nf, col_nb, col_nbo },
	[SchemeSel]  = { col_sf, col_sb,  col_sb  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL, NULL, NULL, 0, False, -1},
	// { "Gimp",     NULL,       NULL,       0,            1,           -1 },
	// { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const float smfact     = 0.00; /* factor of tiled clients [0.00..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "(t)",      tile },    /* first entry is default */
	{ "{f}",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY    Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define HOME "/home/syed"
#define DWMDIR "/home/syed/sr/dwm"

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

/* applications */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_nb, "-nf", col_nf, "-sb", col_sb, "-sf", col_sf, NULL };
static const char *termcmd[]  = { "st", "-d", HOME, "-e", "tmux", NULL};
static const char *nnn[]      = { "st", "-d", "~", "bash", "-c", "nnn -il;bash",   NULL};
static const char *calcurse[] = { "st", "-d", HOME, "bash", "-c", "calcurse;bash",  NULL};
static const char *mutt[]     = { "st", "-d", HOME, "bash", "-c", "mutt;bash",      NULL};
static const char *vim[]      = { "gvim", NULL};
static const char *calc[]    = { "galculator", NULL};
static const char *xkill[]    = { "xkill", NULL};
static const char *lispi[] = { "gvim", HOME "/li.lisp", NULL};
static const char *clipmenu[] = { "clipmenu", "-nb", col_nb, "-nf", col_nf, "-sf", col_sf, "-sb", col_sb, "-fn", dmenufont, NULL};

/*other*/
static const char *brightness_dec[]   = {DWMDIR "/bin/brightness", "5%-", "-", NULL};
static const char *brightness_inc[]   = {DWMDIR "/bin/brightness", "+5%", "+", NULL};
static const char *brightness_1[]   = {DWMDIR "/bin/brightness", "1", "1", NULL};
static const char *brightness_4[]   = {DWMDIR "/bin/brightness", "4", "4", NULL};
static const char *colemak[]          = {DWMDIR "/bin/colemak", NULL};
static const char *qwerty[]           = {DWMDIR "/bin/qwerty", NULL};
static const char *german[]           = {DWMDIR "/bin/german", NULL};
static const char *screenshot[]       = {DWMDIR "/bin/screenshot", "Screenshot", NULL};
static const char *toggle_comp[]      = {DWMDIR "/bin/toggle_comp", NULL};
static const char *volume_force_dec[] = {DWMDIR "/bin/volume_force", "-5%", NULL};
static const char *volume_force_inc[] = {DWMDIR "/bin/volume_force", "+5%", NULL};
static const char *volume_mute[]      = {DWMDIR "/bin/volume", "toggle", NULL};
static const char *shutdown[]         = {"systemctl", "poweroff", NULL};
static const char *suspend[]          = {"systemctl", "suspend", "-i", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	/* window management */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_p,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_p,      fullscreen,     {0} },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_y,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_o,      reorganizetags, {0} },
	{ MODKEY|ShiftMask,             XK_n,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_e,      pushup,         {0} },
	{ MODKEY,                       XK_bracketright,      shiftview, {+1}},
	{ MODKEY,                       XK_bracketleft,      shiftview, {-1}},
	{ MODKEY,                       XK_n,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_e,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_i,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_i,      setsmfact,      {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setsmfact,      {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	/* monitor */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	/* aesthetics */
	{ MODKEY,                       XK_apostrophe,      togglegaps,     {0} },
	{ MODKEY,                       XK_numbersign, spawn, {.v = toggle_comp   } },
	/* brightness */
	{ MODKEY|ShiftMask,             XK_bracketright, spawn, {.v = brightness_inc} },
	{ MODKEY|ShiftMask,             XK_bracketleft,  spawn, {.v = brightness_dec} },
	{ MODKEY,             XK_backslash,  spawn, {.v = brightness_4} },
	{ MODKEY|ShiftMask,             XK_backslash,  spawn, {.v = brightness_1} },
	{ 0,      XF86XK_MonBrightnessUp,   spawn, {.v = brightness_inc} },
	{ 0,      XF86XK_MonBrightnessDown, spawn, {.v = brightness_dec} },
	/* closing things */
	{ MODKEY|ShiftMask,             XK_w,      killclient, {0} },
	{ MODKEY|ShiftMask|ControlMask, XK_q,      quit,  {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,  {1}},
	{ MODKEY|ShiftMask|ControlMask, XK_s,      spawn, {.v = suspend       } },
	{ MODKEY|ShiftMask|ControlMask, XK_Delete, spawn, {.v = shutdown      } },
	/* applications */
	{ MODKEY,                       XK_l,      spawn, {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn, {.v = termcmd } },
	{ MODKEY,                       XK_c,      spawn, {.v = calcurse      } },
	{ MODKEY,                       XK_k,      spawn, {.v = nnn           } },
	{ MODKEY,                       XK_m,      spawn, {.v = mutt          } },
	{ MODKEY,                       XK_v,      spawn, {.v = vim           } },
	{ MODKEY,                       XK_x,      spawn, {.v = calc           } },
	{ MODKEY|ShiftMask,             XK_z,      spawn, {.v = xkill           } },
	{ MODKEY,                       XK_s,      spawn, {.v = screenshot    } },
	{ 0,                            XK_Print,  spawn, {.v = screenshot    } },
	{ MODKEY,                       XK_a,      spawn, {.v = lispi}},
	{ MODKEY,                       XK_slash,  spawn, {.v = clipmenu}},
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	/* keyboard */
	{ MODKEY,                       XK_equal,  spawn, {.v = colemak}},
	{ MODKEY,                       XK_minus,  spawn, {.v = qwerty}},
	{ MODKEY|ShiftMask,             XK_minus,  spawn, {.v = german}},
	{ MODKEY|ShiftMask,	XK_apostrophe, spawn, SHCMD("sxiv ~/me/pictures/cf.png")},
	/* audio */
	{ 0,      XF86XK_AudioRaiseVolume,  spawn, {.v = volume_force_inc} },
	{ 0,      XF86XK_AudioLowerVolume,  spawn, {.v = volume_force_dec} },
	{ 0,      XF86XK_AudioMute,         spawn, {.v = volume_mute   } },
	/* virtual desktops */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
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

