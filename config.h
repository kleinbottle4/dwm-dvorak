/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;
static const unsigned int snap      = 0;       /* snap pixel */
static const unsigned int minwsz    = 20;       /* Minimal heigt of a client for smfact */
static const unsigned int cornerrad = 10;
static const unsigned int gappih    = 4;
static const unsigned int gappiv    = 4;
static const unsigned int gappoh    = 4;
static const unsigned int gappov    = 4;
static const int smartgaps          = 0;
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char dmenufont[]       = "Inconsolata:size=20";
static const char *fonts[]          = { dmenufont };
static const char col_nb[] = "gray10";
static const char col_nbo[] = "gray10";
static const char col_nf[] = "white";
static const char col_sf[] = "white";
static const char col_sb[] = "gray5";
static const char col_sbo[] = "white";
static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_nf, col_nb, col_nbo },
	[SchemeSel]  = { col_sf, col_sb,  col_sbo  },
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
	{ "!!!",      fullmonocle }, /* fullscreen - bar should not be visible */
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
static const char *nnn[]      = { "st", "-d", "~", "bash", "-c", "nnn -i; bash",    NULL};
static const char *w3m[]      = { "st", "-d", "~", "bash", "-c", "w3m -v; bash",    NULL};
static const char *calcurse[] = { "st", "-d", HOME, "bash", "-c", "calcurse; bash", NULL};
static const char *mutt[]     = { "st", "-d", HOME, "bash", "-c", "mutt; bash",     NULL};
static const char *emacs[]    = { "st", "-d", HOME, "bash", "-c", "emacs -Q li.elisp --eval \"(lisp-interaction-mode)\"; bash", NULL};
static const char *vim[]      = { "st", "-d", HOME, "bash", "-c", "vim; bash",      NULL};
static const char *calc[]     = { "galculator", NULL};
static const char *xkill[]    = { "xkill", NULL};
static const char *clipmenu[] = { "clipmenu", "-nb", col_nb, "-nf", col_nf, "-sf", col_sf, "-sb", col_sb, "-fn", dmenufont, NULL};
static const char *clipdel[]  = { "clipdel", "-d", ".*", NULL };

/*other*/
static const char *brdec[] = {"/usr/bin/brightnessctl", "set", "5%-", NULL };
static const char *brinc[] = {"/usr/bin/brightnessctl", "set", "+5%", NULL };
static const char *br1[] = {"/usr/bin/brightnessctl", "set", "1", NULL };
static const char *br4[] = {"/usr/bin/brightnessctl", "set", "4", NULL };
static const char *maimsel[] = {DWMDIR "/bin/maim", "Screenshot", "-s", NULL};
static const char *maim[] = {DWMDIR "/bin/maim", "Screenshot", "", NULL};
static const char *toggle_comp[] = {DWMDIR "/bin/toggle_comp", NULL};
static const char *volfinc[] = { "pactl", "set-sink-volume", "alsa_output.pci-0000_00_14.2.analog-stereo", "+5%", NULL};
static const char *volfdec[] = { "pactl", "set-sink-volume", "alsa_output.pci-0000_00_14.2.analog-stereo", "-5%", NULL};
static const char *volmut[] = { "amixer", "-M", "set", "Master", "toggle", NULL};
static const char *volinc[] = { "amixer", "-M", "set", "Master", "5%+", NULL};
static const char *voldec[] = { "amixer", "-M", "set", "Master", "5%-", NULL};
static const char *shutdown[] = {"systemctl", "poweroff", NULL};
static const char *suspend[] = {"systemctl", "suspend", "-i", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	/* window management */
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_f,      fullscreen,     {0} },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_o,      reorganizetags, {0} },
	{ MODKEY|ShiftMask,             XK_o,      distributetags, {0} },
	{ MODKEY,                       XK_d,      shiftview, {-1}},
	{ MODKEY,                       XK_h,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_t,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_n,      shiftview, {+1}},
	{ MODKEY|ShiftMask,             XK_h,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_t,      pushup,         {0} },
	{ MODKEY,                       XK_Left,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_Right,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Left,      setsmfact,      {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_Right,      setsmfact,      {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	/* monitor */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	/* aesthetics: gaps, compositor, brightness */
	{ MODKEY,                       XK_apostrophe,      togglegaps,     {0} },
	{ MODKEY,                       XK_apostrophe,      notifysend, {.v = "Toggle gaps"}},
	{ MODKEY|ShiftMask,             XK_apostrophe,      spawn, {.v = toggle_comp}},
	{ 0, XF86XK_MonBrightnessDown,       spawn,      {.v = brdec} },
	{ 0, XF86XK_MonBrightnessDown,       notifysend, {.v = "Brightness -"}},
	{ 0, XF86XK_MonBrightnessUp,         spawn,      {.v = brinc} },
	{ 0, XF86XK_MonBrightnessUp,         notifysend, {.v = "Brightness +"}},
	{ MODKEY,           XK_minus,        spawn,      {.v = brdec} },
	{ MODKEY,           XK_minus,        notifysend, {.v = "Brightness -"}},
	{ MODKEY|ShiftMask, XK_minus,        spawn,      {.v = brinc} },
	{ MODKEY|ShiftMask, XK_minus,        notifysend, {.v = "Brightness +"}},
	{ MODKEY,           XK_numbersign,   spawn,      {.v = br4} },
	{ MODKEY,           XK_numbersign,   notifysend, {.v = "Brightness 4"}},
	{ MODKEY|ShiftMask, XK_numbersign,   spawn,      {.v = br1} },
	{ MODKEY|ShiftMask, XK_numbersign,   notifysend, {.v = "Brightness 1"}},
	/* closing things */
	{ MODKEY|ShiftMask,             XK_c,      killclient, {0} },
	{ MODKEY|ShiftMask|ControlMask, XK_q,      quit,  {0} },
	{ MODKEY|ShiftMask,             XK_r,      quit,  {1}},
	{ MODKEY|ShiftMask|ControlMask, XK_z,      spawn, {.v = suspend}},
	{ MODKEY|ShiftMask,             XK_Delete, spawn, {.v = shutdown}},
	/* applications */
	{ MODKEY,                       XK_w, spawn, {.v = w3m }},
	{ MODKEY,                       XK_p,      spawn, {.v = dmenucmd}},
	{ MODKEY|ShiftMask,             XK_Return, spawn, {.v = termcmd}},
	{ MODKEY,                       XK_c,      spawn, {.v = calcurse}},
	{ MODKEY,                       XK_k,      spawn, {.v = nnn}},
	{ MODKEY,                       XK_a,      spawn, {.v = mutt}},
	{ MODKEY,                       XK_v,      spawn, {.v = vim}},
	{ MODKEY,                       XK_g,      spawn, {.v = calc}},
	{ MODKEY|ShiftMask,             XK_x,      spawn, {.v = xkill}},
	{ MODKEY,                       XK_s,      spawn, {.v = maim}},
	{ MODKEY|ShiftMask,             XK_s,      spawn, {.v = maimsel}},
	{ 0,                            XK_Print,  spawn, {.v = maim}},
	{ MODKEY,                       XK_e,      spawn, {.v = emacs}},
	{ MODKEY,                       XK_slash,  spawn, {.v = clipmenu}},
	{ MODKEY|ShiftMask,             XK_slash,  spawn, {.v = clipdel}},
	{ MODKEY,                       XK_b,      togglebar, {0} },
	{ MODKEY,                       XK_x,      spawn, SHCMD("xterm")},
	{ MODKEY,                       XK_j,      spawn, SHCMD("lxterminal")},
	{ MODKEY|ShiftMask,             XK_backslash,      spawn, SHCMD("qw; notify-send qw")},
	{ MODKEY,                       XK_backslash,      spawn, SHCMD("dvp; notify-send dvp")},
	{ MODKEY,                       XK_at,      spawn, SHCMD("sxiv dvp.png")},
	/* audio */
	{ 0,      XF86XK_AudioRaiseVolume,  spawn, {.v = volinc}},
	{ 0,      XF86XK_AudioLowerVolume,  spawn, {.v = voldec}},
	{ 0,      XF86XK_AudioMute,         spawn, {.v = volmut}},
	// { MODKEY, XK_equal,                 spawn, {.v = volfinc }},
	// { MODKEY, XK_minus,                 spawn, {.v = volfdec }},
	{ 0,      XF86XK_AudioRaiseVolume,  spawn, SHCMD(DWMDIR "/bin/volume_notify") },
	{ 0,      XF86XK_AudioLowerVolume,  spawn, SHCMD(DWMDIR "/bin/volume_notify") },
	{ 0,      XF86XK_AudioMute,         spawn, SHCMD(DWMDIR "/bin/volume_notify toggle") },
	// { MODKEY, XK_equal,                 spawn, SHCMD(DWMDIR "/bin/volume_notify") },
	// { MODKEY, XK_minus,                 spawn, SHCMD(DWMDIR "/bin/volume_notify") },
	/* virtual desktops */
	{ MODKEY,                       XK_asterisk,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_asterisk,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_parenleft,              0)
	TAGKEYS(                        XK_parenright,             1)
	TAGKEYS(                        XK_braceright,             2)
	TAGKEYS(                        XK_plus,                   3)
	TAGKEYS(                        XK_braceleft,              4)
	TAGKEYS(                        XK_bracketright,           5)
	TAGKEYS(                        XK_bracketleft,            6)
	TAGKEYS(                        XK_exclam,                 7)
	TAGKEYS(                        XK_equal,                  8)
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

