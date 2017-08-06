
//
// Defines for sound
//
const int SND_PAN_CENTER = 0.0f;
const int SND_PAN_LEFT = -1.0f;
const int SND_PAN_RIGHT = 1.0f;

//
// Font sizes
//
const int FONT_MEDIUM = 0;
const int FONT_LARGE = 1;
const int FONT_GUI = 2;

const int GUI_OBJECT_SCREEN = 1;
const int GUI_OBJECT_BUTTON = 2;
const int GUI_OBJECT_CHECKBOX = 3;
const int GUI_OBJECT_TEXT_LABEL = 4;
const int GUI_OBJECT_POPUP = 5;
const int GUI_OBJECT_IMAGE = 6;
const int GUI_OBJECT_TEXT_BOX = 7;
const int GUI_OBJECT_SLIDER = 8;
const int GUI_OBJECT_KEYCODE = 9;

// Index into which color is being set
const int GUI_BUTTON_ACTIVE_COL = 0;
const int GUI_BUTTON_INACTIVE_COL = 1;
const int GUI_BUTTON_ACTIVE_EDGE_COL = 2;
const int GUI_BUTTON_INACTIVE_EDGE_COL = 3;
const int GUI_BUTTON_ACTIVE_LABEL_COL = 4;
const int GUI_BUTTON_INACTIVE_LABEL_COL = 5;

const int GUI_CHECKBOX_TICKED_COL = 6;
const int GUI_CHECKBOX_NOT_TICKED_COL = 7;
const int GUI_OBJECT_TEXT_LABEL_COLOR = 8;

const int GUI_COORD_TYPE_PIXEL = 0;
const int GUI_COORD_TYPE_PERCENT = 1;

const int GUI_LABEL_CENTER = 0;
const int GUI_LABEL_LEFT = 1;
const int GUI_LABEL_RIGHT = 2;

const int GUI_SLIDER_STRING = 0;
const int GUI_SLIDER_INT = 1;
const int GUI_SLIDER_FLOAT = 2;

const int GUI_SLIDER_BAR_COL_ACTIVE = 0;
const int GUI_SLIDER_BAR_COL_NOT_ACTIVE = 1;

const int GUI_SLIDER_SELECTOR_COL_ACTIVE = 2;
const int GUI_SLIDER_SELECTOR_COL_NOT_ACTIVE = 3;

const int GUI_SLIDER_LABEL_COL_ACTIVE = 4;
const int GUI_SLIDER_LABEL_COL_NOT_ACTIVE = 5;

const int GUI_SLIDER_VALUE_COL_ACTIVE = 6;
const int GUI_SLIDER_VALUE_COL_NOT_ACTIVE = 7;

const int MODE_CONSOLE = 1;
const int MODE_SHOWLEVEL = 2;
const int MODE_LOADING = 3;
const int MODE_LIFT_VIEW = 4;
const int MODE_SHOW_SPLASH = 5;
const int MODE_TERMINAL_MENU = 6;
const int MODE_TERMINAL_SIDEVIEW = 7;
const int MODE_TERMINAL_LEVEL = 8;
const int MODE_TERMINAL_DB = 9;
const int MODE_GUI = 10;
const int MODE_INTRO = 11;
//const int  MODE_TRANSFER				= 12;
const int MODE_TRANSFER_INTRO = 13;
const int MODE_TRANSFER_INTRO_1 = 14;
const int MODE_TRANSFER_INTRO_2 = 15;
const int MODE_TRANSFER_FINISH = 16;
const int MODE_TRANSFER_START = 17;
const int MODE_TRANSFER_COPY = 18;
const int MODE_TRANSFER_DEADLOCK = 19;
const int MODE_TRANSFER_SELECT = 20;
const int MODE_TRANSFER_SELECT_SIDE = 21;
const int MODE_PAUSED = 22;
const int MODE_QUIT = 23;
const int MODE_KEYCODE = 24;
const int MODE_END_STATIC_PRE = 25;
const int MODE_END_STATIC = 26;
const int MODE_END_TEXT = 27;
const int MODE_END_TEXT_FINISH = 28;
const int MODE_PLAYER_EXPLODE = 29;
const int MODE_CONFIRM_QUIT = 30;
const int MODE_TRANSFER_LOST = 31;
const int MODE_BEAM_ON = 32;

const int NUM_OF_LEVELS = 21;

enum actionNames
{
    gameLeft = 0,
    gameRight,
    gameUp,
    gameDown,
    consoleAction,
    gameFire,
    gameEscape,
    actionScreenShot,
    gamePause,
    NUMBER_ACTIONS
};

enum mySounds
{
    SND_COLLIDE_1 = 0,
    SND_END_TRANSMISSION_1,
    SND_GREEN_ALERT,
    SND_LIFT_1,
    SND_SCROLLBEEPS,
    SND_TRANSFER_DEADLOCK,
    SND_YELLOW_ALERT,
    SND_CONSOLE_1,
    SND_END_TRANSMISSION_2,
    SND_KEYPRESS_BAD,
    SND_LIFT_2,
    SND_START_1,
    SND_TRANSFER_SELECT,
    SND_CONSOLE_2,
    SND_ENERGY_HEAL,
    SND_KEYPRESS_GOOD,
    SND_LIFT_3,
    SND_START_2,
    SND_TRANSFER_STAGE_1,
    SND_DAMAGE,
    SND_EXPLODE_1,
    SND_LASER,
    SND_LOW_ENERGY,
    SND_START_ALL,
    SND_TRANSFER_STAGE_2,
    SND_DISRUPTOR,
    SND_EXPLODE_2,
    SND_LEVEL_SHUTDOWN,
    SND_RED_ALERT,
    SND_TRANSFER_1,
    SND_TRANSFER_START,
    SND_DOOR,
    NUM_SOUNDS
};

//
// Defines for what type of particle to draw
// Primitive is using al_draw_circle
//
const int PARTICLE_RENDER_OFF = 0;
const int PARTICLE_RENDER_BITMAP = 1;
const int PARTICLE_RENDER_PRIMITIVE = 2;

const int PARTICLE_TYPE_EXPLOSION = 0;
const int PARTICLE_TYPE_TRAIL = 1;
const int PARTICLE_TYPE_SPARK = 2;

//
// Defines for different languages and strings
//
const int LANG_ENGLISH = 0;
const int LANG_ITALIAN = 1;

//
// Defines for input types
//
const int INPUT_KEYBOARD = 0;
const int INPUT_JOYSTICK = 1;

//
// Used in transfer AI SCRIPT
//
const int POWER_YELLOW =		1;
const int POWER_PURPLE =		2;
const int POWER_DISPUTE = 		3;


const int CELL_HEIGHT	= 32;
const int NUM_CELLS	= 16;

const int LEFT_SIDE = 1;
const int RIGHT_SIDE = 2;

const int TRUE	= 1;
const int FALSE	= 0;

const int CIRCUIT_REVERSED	= 1;
const int CIRCUIT_REPEATER	= 2;

const int CIRCUIT_NORMAL	= 1;
const int CIRCUIT_THREEQUARTERS	= 2;
const int CIRCUIT_HALF	= 3;
const int CIRCUIT_QUARTER	= 4;

const int CIRCUIT_TWO_INTO_ONE	= 5;

const int CIRCUIT_ONE_INTO_TWO	= 6;
const int CIRCUIT_NORMAL_1	= 7;
const int CIRCUIT_NORMAL_2	= 8;

const int CIRCUIT_SPLIT_HALF	= 16;
const int CIRCUIT_ONE_INTO_TWO_SHORT	= 17;
const int CIRCUIT_SPLIT_TWO_INTO_ONE	= 18;

const int NUM_CIRCUITS	= 8;

const int MAX_EFFECT	= 6;
