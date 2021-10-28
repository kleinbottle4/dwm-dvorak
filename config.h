/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

#define COL2 "#ecf0c1"
#define COL1 "#0a1e24"
//"gray5"
static const char     col_nb[]     = COL1;
static const char     col_nf[]     = COL2;
static const char     col_nbo[]    = "black";
static const char     col_sb[]     = COL2;
static const char     col_sf[]     = COL1;
static const char     col_sbo[]    = "white";
static const char     *colors[][3] = {[SchemeNorm] = {col_nf, col_nb, col_nbo}, [SchemeSel]  = {col_sf, col_sb,  col_sbo }, };
static const char     dmenufont[]  = "monospace:size=18";
static const char     *fonts[]     = {dmenufont};
static const float    mfact        = 0.55; /* factor of master area size [0.05..0.95] */
static const float    smfact       = 0.00; /* factor of tiled clients [0.00..0.95] */
static const int      nmaster      = 1; /* number of clients in master area */
static const int      resizehints  = 0; /* 1 means respect size hints in tiled resizals */
static const int      showbar      = 1;
static const int      topbar       = 0;
static const int      smartgaps    = 0;
static const unsigned int borderpx = 2;
static const unsigned int gappih   = 4;
static const unsigned int gappiv   = 4;
static const unsigned int gappoh   = 4;
static const unsigned int gappov   = 4;
static const unsigned int minwsz   = 20; /* Minimal heigt of a client for smfact */
static const unsigned int snap     = 0;

static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
static const char ptagf[] = "%s%s";	/* format of a tag label */
static const char etagf[] = "%s";	/* format of an empty tag */
static const int lcaselbl = 0;		/* 1 means make tag label lowercase */	

static const Layout layouts[] = {{"(t)", tile}, {"{f}", NULL}, {"[M]", monocle}, {"FUL", fullmonocle}, {"<d>", deck} };
static const Rule rules[] = {{NULL, NULL, NULL, 0, False, -1} };

/* key definitions */
#define MODKEY    Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{MODKEY,                       KEY,      view,           {.ui = 1 << TAG}}, \
	{MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG}}, \
	{MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG}}, \
	{MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG}},

#define SHCMD(cmd) {.v = (const char*[]){"/bin/sh", "-c", cmd, NULL}}
#define NOT(s) "notify-send -t 1000 " s
#define ST(s) SHCMD("st -d " HOME " " s)
#define TMUX(s) SHCMD("tmux new-session " s)
#define HOME "/home/syed"
#define DWMDIR "/home/syed/src/dwm"

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_nb, "-nf", col_nf, "-sb", col_sb, "-sf", col_sf, NULL};
static const char *termcmd[] = {"st", "-d", HOME, "tmux", NULL};
static const char *termcmdalt[] = {"st", "-d", HOME, "9", "rc", NULL};
static const char *clipmenu[] = {"clipmenu", "-nb", col_nb, "-nf", col_nf, "-sf", col_sf, "-sb", col_sb, "-fn", dmenufont, NULL};

#define VOLF(s) SHCMD("pactl set-sink-volume alsa_output.pci-0000_00_14.2.analog-stereo " \
	s " && " DWMDIR "/bin/volume_notify ")

#define VOL(s) SHCMD("amixer -M set Master " \
	s " && " DWMDIR "/bin/volume_notify")

static Key keys[] = {
	/* window management */
	{MODKEY|ShiftMask,             XK_o,      distributetags, {0}},
	{MODKEY,                       XK_h,      focusstack,     {.i = +1}},
	{MODKEY,                       XK_t,      focusstack,     {.i = -1}},
	{MODKEY|ShiftMask,             XK_f,      fullscreen,     {0}},
	{MODKEY,                       XK_i,      incnmaster,     {.i = +1}},
	{MODKEY,                       XK_u,      incnmaster,     {.i = -1}},
	{MODKEY|ShiftMask,             XK_h,      pushdown,       {0}},
	{MODKEY|ShiftMask,             XK_t,      pushup,         {0}},
	{MODKEY,                       XK_o,      reorganizetags, {0}},
	{MODKEY,                       XK_l,      setlayout,      {.v = &layouts[0]}},
	{MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]}},
	{MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]}},
	{MODKEY|ShiftMask,             XK_d,      setlayout,      {.v = &layouts[4]}},
	{MODKEY,                       XK_space,  setlayout,      {0}},
	{MODKEY,                       XK_Right,  setmfact,       {.f = +0.05}},
	{MODKEY,                       XK_Left,   setmfact,       {.f = -0.05}},
	{MODKEY,                       XK_Up,     setsmfact,      {.f = +0.05}},
	{MODKEY,                       XK_Down,   setsmfact,      {.f = -0.05}},
	{MODKEY,                       XK_n,      shiftview,      {+1}},
	{MODKEY,                       XK_d,      shiftview,      {-1}},
	{MODKEY,                       XK_b,      togglebar, {0}},
	{MODKEY|ShiftMask,             XK_space,  togglefloating, {0}},
	{MODKEY,                       XK_Tab,    view,           {0}},
	{MODKEY,                       XK_Return, zoom,           {0}},

	/* monitor */
	{MODKEY,                       XK_comma,  focusmon,       {.i = -1}},
	{MODKEY,                       XK_period, focusmon,       {.i = +1}},
	{MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1}},
	{MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1}},

	/* aesthetics: gaps, compositor, brightness */
//	{MODKEY,                    XK_backslash, spawn, SHCMD("pkill xcompmgr &&" NOT("kill comp"))},
//	{MODKEY|ShiftMask,          XK_backslash, spawn, SHCMD("xcompmgr &&" NOT("comp"))},
	{MODKEY, XK_backslash, spawn, SHCMD("pgrep xcompmgr && pkill xcompmgr || xcompmgr; " NOT("comp"))},
//	{MODKEY,                   XK_apostrophe, togglegaps, {0}},
//	{MODKEY,                   XK_apostrophe, spawn,      SHCMD(NOT("gaps"))},
	{MODKEY|ShiftMask,         XK_numbersign,      spawn,      SHCMD("brightnessctl set 1 &&" NOT("br\\ 1"))},
	{MODKEY,                   XK_numbersign,      spawn,      SHCMD("brightnessctl set 4 &&" NOT("br\\ 4"))},
	{MODKEY,                   XK_minus,      spawn,      SHCMD("brightnessctl set 5%+ &&" NOT("br\\ +"))},
	{MODKEY|ShiftMask,         XK_minus,      spawn,      SHCMD("brightnessctl set 5%- &&" NOT("br\\ -"))},
	{0,             XF86XK_MonBrightnessUp,   spawn,      SHCMD("brightnessctl set 5%+ &&" NOT("br\\ +"))},
	{0,             XF86XK_MonBrightnessDown, spawn,      SHCMD("brightnessctl set 5%- &&" NOT("br\\ -"))},

	/* closing things */
	{MODKEY|ShiftMask,             XK_c,      killclient, {0}},
	{MODKEY|ShiftMask|ControlMask, XK_q,      quit,  {0}},
	{MODKEY|ShiftMask,             XK_r,      quit,  {1}},
	{MODKEY|ShiftMask,             XK_Delete, spawn, SHCMD("systemctl poweroff")},
	{MODKEY,                       XK_z,      spawn, ST("systemctl -i suspend")},

	/* applications */
	{MODKEY|ShiftMask,             XK_l,      spawn, SHCMD("slock")},
	{MODKEY|ShiftMask,             XK_x,      spawn, SHCMD("xkill")},
	{MODKEY|ShiftMask|ControlMask, XK_s,      spawn, SHCMD(DWMDIR "/bin/maim Screenshot -s")},
	{MODKEY|ShiftMask,             XK_s,      spawn, SHCMD(DWMDIR "/bin/maim Screenshot")},
	{0,                            XK_Print,  spawn, SHCMD(DWMDIR "/bin/maim Screenshot")},
	{MODKEY,                       XK_c,      spawn, ST("calcurse")},
	{MODKEY,                       XK_e,      spawn, SHCMD("emacsclient -c -a \"\" ~/foo/org/main.org")},
	{MODKEY,                       XK_j,      spawn, ST("nnn -i")},
	{MODKEY,                       XK_slash,  spawn, {.v = clipmenu}},
	{MODKEY,                       XK_p,      spawn, {.v = dmenucmd}},
	{MODKEY|ShiftMask,             XK_Return, spawn, {.v = termcmd}},
	{MODKEY,                       XK_r,      spawn, {.v = termcmdalt}},

	/* audio */
	{0,                            XF86XK_AudioRaiseVolume,  spawn, VOLF("+5%")},
	{0,                            XF86XK_AudioLowerVolume,  spawn, VOLF("-5%")},
	{0,                            XF86XK_AudioMute,         spawn, VOL("toggle")},

	/* virtual desktops */
	{MODKEY,                       XK_asterisk,     view, {.ui = ~0}},
	{MODKEY|ShiftMask,             XK_asterisk,     tag,  {.ui = ~0}},
	TAGKEYS(                       XK_parenleft,    0)
	TAGKEYS(                       XK_parenright,   1)
	TAGKEYS(                       XK_braceright,   2)
	TAGKEYS(                       XK_plus,         3)
	TAGKEYS(                       XK_braceleft,    4)
	TAGKEYS(                       XK_bracketright, 5)
	TAGKEYS(                       XK_bracketleft,  6)
	TAGKEYS(                       XK_exclam,       7)
	TAGKEYS(                       XK_equal,        8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click               event mask      button          function        argument */
	{ClkLtSymbol,          0,              Button1,        setlayout,      {0}},
	{ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]}},
	{ClkWinTitle,          0,              Button2,        zoom,           {0}},
	{ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd}},
	{ClkClientWin,         MODKEY,         Button1,        movemouse,      {0}},
	{ClkClientWin,         MODKEY,         Button2,        togglefloating, {0}},
	{ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0}},
	{ClkTagBar,            0,              Button1,        view,           {0}},
	{ClkTagBar,            0,              Button3,        toggleview,     {0}},
	{ClkTagBar,            MODKEY,         Button1,        tag,            {0}},
	{ClkTagBar,            MODKEY,         Button3,        toggletag,      {0}},
};

