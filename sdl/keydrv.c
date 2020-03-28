/* KEYDRV.C
  The low-level keyboard driver for the SDL port.
  
  Sorry I know the mapping stuff is kind of confusing.
  I think that's just the way it goes.
  It works though right?
*/
#define SWITCH

#ifdef SWITCH
//#define SDLK_LAST 9999999
#define MAP_SIZE 255
#endif

#include "../keen.h"
#include <SDL.h>
#include "keydrv.fdh"


struct Map {
    int key;
    int value;
};






int DO_QUIT = 0;

int mouse_x, mouse_y;
int mouseL, mouseR;

int setting_keymap_ktindex = 0;
int setting_keymap_keyno = 0;

int sdl_keysdown_head = -1;
struct Map sdl_keysdown[MAP_SIZE];		// this is used by the menus
//int sdl_keysdown[SDLK_LAST];		// this is used by the menus

int sdl_lastkeysdown_head = -1;
struct Map sdl_lastkeysdown[MAP_SIZE];	// this is used by the editor
//int sdl_lastkeysdown[SDLK_LAST];	// this is used by the editor

// this is a lookup table of SDL keys -> keytable keys.
// for example keymappings[SDLK_LEFT] might contain the value 'KLEFT'.
//int keymappings[SDLK_LAST];

int keymappings_head = -1;
struct Map keymappings[MAP_SIZE];

#define MAX_KEYS_PER_CONTROL		2
typedef struct
{
	int sdl_keysym[MAX_KEYS_PER_CONTROL];
} stRevKeyMap;
stRevKeyMap reverse_keymappings[KEYTABLE_SIZE];

void set_value(struct Map map[], int* head, int key, int value)
{
	int i=0;
	for(i=0;i<= *head; i++)
	{
		if(key == map[i].key)
		{
			map[i].value = value;
			return;
		}
	}

	(*head)++;
	map[*head].key = key;
	map[*head].value = value;

}

int do_print = 0;
int get_value(struct Map map[], int* head, int key)
{
    /* combine all inputs in some way */
    int i=0;
	if(do_print)
	{	
		//for(i=0; i<=*head; i++)
		{
		//	lprintf("(%d,%d) , ",map[i].key,map[i].value);
		}
		//lprintf("\n");
	}
	for(i=0;i<= *head; i++)
	{
		if(key == map[i].key)
		{
			return map[i].value;
		}
	}
	return 0;	
}

// switch button to number
#define JOY_A 0
#define	JOY_B 1
#define JOY_X 2
#define JOY_Y 3
#define JOY_R1 7
#define JOY_R2 9
#define JOY_R3 5
#define JOY_L1 6
#define JOY_L2 8
#define JOY_L3 4
//#define Joy_Y-s -> Axis 1 ( up is neg) 
//#define Joy_X-s -> Axis 0 ( right is pos)
#define JOY_LEFT 12
#define JOY_UP 13
#define JOY_RIGHT 14
#define JOY_DOWN 15
#define JOY_MINUS 11
#define JOY_PLUS 10


void KeyDrv_SetDefaultMappings(void)
{
	
	//lprintf("keydrv: Setting default keyboard mappings.\n");
	
	//memset(keymappings, 0, sizeof(keymappings));
	keymappings_head = -1;
	set_value(keymappings, &keymappings_head, JOY_LEFT, KLEFT);
	set_value(keymappings, &keymappings_head, JOY_RIGHT, KRIGHT);
	set_value(keymappings, &keymappings_head, JOY_UP, KUP);
	set_value(keymappings, &keymappings_head, JOY_DOWN, KDOWN);

	//lprintf("%d,%d,%d\n", keymappings_head, 15, get_value(keymappings, &keymappings_head, 15) );
	set_value(keymappings, &keymappings_head, JOY_X, KCTRL);//set_value(keymappings, &keymappings_head, SDLK_RALT, KCTRL);
	set_value(keymappings, &keymappings_head, JOY_Y, KALT);	//set_value(keymappings, &keymappings_head, SDLK_RCTRL, KALT);
	set_value(keymappings, &keymappings_head, JOY_A, KSPACE);
	set_value(keymappings, &keymappings_head, JOY_B, KENTER);

	set_value(keymappings, &keymappings_head, JOY_MINUS, KESC);
	set_value(keymappings, &keymappings_head, JOY_PLUS, KDEL);


	/*keymappings[SDLK_LEFT] = KLEFT;
	keymappings[SDLK_RIGHT] = KRIGHT;
	keymappings[SDLK_UP] = KUP;
	keymappings[SDLK_DOWN] = KDOWN;
	keymappings[SDLK_LCTRL] = KCTRL; keymappings[SDLK_RALT] = KCTRL;
	keymappings[SDLK_LALT] = KALT; keymappings[SDLK_RCTRL] = KALT;
	keymappings[SDLK_SPACE] = KSPACE;
	keymappings[SDLK_RETURN] = KENTER;
	
	keymappings[SDLK_ESCAPE] = KESC;
	keymappings[SDLK_DELETE] = KDEL;*/
	
	set_value(keymappings, &keymappings_head, JOY_L1, KPLUS); set_value(keymappings, &keymappings_head, SDLK_EQUALS, KPLUS);
	set_value(keymappings, &keymappings_head, JOY_R1, KMINUS); set_value(keymappings, &keymappings_head, SDLK_MINUS, KMINUS);
	set_value(keymappings, &keymappings_head, JOY_L2, KTAB);
	
	set_value(keymappings, &keymappings_head, JOY_R2, KF1);
	set_value(keymappings, &keymappings_head, JOY_L3, KF2);
	set_value(keymappings, &keymappings_head, JOY_R3, KF3);
	set_value(keymappings, &keymappings_head, SDLK_F4, KF4);
	set_value(keymappings, &keymappings_head, SDLK_F5, KF5);
	set_value(keymappings, &keymappings_head, SDLK_F6, KF6);
	set_value(keymappings, &keymappings_head, SDLK_F7, KF7);
	set_value(keymappings, &keymappings_head, SDLK_F8, KF8);
	set_value(keymappings, &keymappings_head, SDLK_F9, KF9);
	set_value(keymappings, &keymappings_head, SDLK_F1,KF10);
	//lprintf(" head at %d\n", keymappings_head);
	
	mappings_to_reverse_mappings();
}

void static mappings_to_reverse_mappings(void)
{
	
int i,j,k;
	//lprintf("keydrv: Generating reverse mapping chart.\n");
	for(i=0;i<KEYTABLE_SIZE;i++)
	{
		for(k=0;k<MAX_KEYS_PER_CONTROL;k++)
			reverse_keymappings[i].sdl_keysym[k] = 0;
			
		k = 0;
		// backwards so Lctrl comes up before Rctrl
		for(j=MAP_SIZE-1;j>=0;j--)
		{
			if(get_value(keymappings, &keymappings_head, j) == i)
			//if (keymappings[j]==i)
			{
				reverse_keymappings[i].sdl_keysym[k] = j;
				if (++k >= MAX_KEYS_PER_CONTROL)
				{
					break;
				}
			}
		}
	}
	
}

void static reverse_mappings_to_mappings(void)
{
		lprintf("keydrv: reverse mappings to mappings.\n");

int i,k;
	memset(keymappings, 0, sizeof(keymappings));
	
	for(i=0;i<KEYTABLE_SIZE;i++)
	{
		for(k=0;k<MAX_KEYS_PER_CONTROL;k++)
		{
			if (reverse_keymappings[i].sdl_keysym[k])
			{
				set_value( keymappings, &keymappings_head, reverse_keymappings[i].sdl_keysym[k],i);
			}
		}
	}
	
}

// try to load the key mappings from the config file, or if we can't,
// set the defaults.
void KeyDrv_LoadKeyMappings(void)
{
		lprintf("keydrv: load key mappings.\n");
	
	int i,k;
	char key[40];
	//if (Ini_GetNumericKey(NULL, "KEYMAPVER") != KEYMAPPINGVERSION)
	{
		KeyDrv_SetDefaultMappings();
		KeyDrv_SaveKeyMappings();
		//reverse_mappings_to_mappings();

		return;
	}
	
	memset(reverse_keymappings, NOKEY, sizeof(reverse_keymappings));
	for(i=1;i<KEYTABLE_SIZE;i++)	// start at 1 because 0=NOKEY
	{
		for(k=0;k<MAX_KEYS_PER_CONTROL;k++)
		{
			sprintf(key, "KEYMAP%d_%d", i, k+1);
			reverse_keymappings[i].sdl_keysym[k] = Ini_GetNumericKey(NULL, key);
			if (reverse_keymappings[i].sdl_keysym[k]==-1)
			{
				// don't save, this way if it was caused by accidental
				// editing of the config file it can be easily fixed.
				// if it's corrupted some other reason, then when the user
				// resets up the keyboard it'll be fixed then.
				KeyDrv_SetDefaultMappings();
				return;
			}
		}
	}
	//reverse_mappings_to_mappings();
	
}

// save the current keymappings to the config file
void KeyDrv_SaveKeyMappings(void)
{
		lprintf("keydrv: save key mappings.\n");
	
int i,k;
char key[40];
	Ini_WriteNumericKey(NULL, "KEYMAPVER", KEYMAPPINGVERSION);
	
	for(i=1;i<KEYTABLE_SIZE;i++)	// start at 1 because 0=NOKEY
	{
		for(k=0;k<MAX_KEYS_PER_CONTROL;k++)
		{
			sprintf(key, "KEYMAP%d_%d", i, k+1);
			Ini_WriteNumericKey(NULL, key, reverse_keymappings[i].sdl_keysym[k]);
		}
	}
}



// returns a text string describing the name of which actual key on the
// keyboard is currently mapped to keytable[keytable_index].
// key_no defines whether to retrieve the first or second key which is
// mapped to that keytable index.
// if no key is mapped at the specified position, returns NULL.
char *noksym = "";
char *beingset = "*****";
char knbuf[40];
const char *KeyDrv_GetKeyMapping(int keytable_index, int key_no)
{
	//return 0;
		lprintf("keydrv: get key mappings.\n");
	
int ksym;


	if (KeyDrv_KeyMappingBeingSet(keytable_index, key_no))
	{
		return beingset;
	}
	
	ksym = reverse_keymappings[keytable_index].sdl_keysym[key_no];
	if (!ksym) return noksym;
	
	const char *kname = SDL_GetKeyName(ksym);
	// change e.g. "left ctrl" to "Lctrl".
	if (strstr(kname, "right "))
	{
		sprintf(knbuf, "R%s", &kname[6]);
		return knbuf;
	}
	else if (strstr(kname, "left "))
	{
		sprintf(knbuf, "L%s", &kname[5]);
		return knbuf;
	}
	return kname;
	
}

// used in menus (so you can't screw up being able to navigate the menu
// by messing up the keyboard mapping).
int KeyDrv_KeyIsDown(int sdl_key)
{
	//return 0;
	//	lprintf("keydrv: key down.\n");
	//	lprintf("%d\n",sizeof(int));
	//lprintf("keydrv: checking key %d\n",sdl_key);
	return get_value(sdl_keysdown, &sdl_keysdown_head, sdl_key);
	//return sdl_keysdown[sdl_key];
}

int KeyDrv_LastKeyIsDown(int sdl_key)
{
		lprintf("keydrv: last key down.\n");
	//return 0;
	return get_value(sdl_lastkeysdown, &sdl_lastkeysdown_head, sdl_key);
	//return sdl_lastkeysdown[sdl_key];
}

void KeyDrv_CopyLastKeys(void)
{
		lprintf("keydrv: copy last key.\n");
		memcpy(sdl_lastkeysdown, sdl_keysdown, sizeof(sdl_lastkeysdown));
}

// initilizes KeyDrv_SetKeyMapping
int lastkeychanged = -1;
void KeyDrv_EnteredKeySetupMenu(void)
{
		lprintf("keydrv: key setup menu.\n");

	lastkeychanged = -1;
}

// specifies that the next key to be pushed down is to become the new
// key to control the specified keytable[] index.
void KeyDrv_SetKeyMapping(int keytable_index)
{
		lprintf("keydrv: set key mapping.\n");
	
int knum;
	if (keytable_index==lastkeychanged)
	{
		knum = 1;
		lastkeychanged = -1;
	}
	else
	{
		knum = 0;
		lastkeychanged = keytable_index;
	}
	setting_keymap_ktindex = keytable_index;
	setting_keymap_keyno = knum;
	
}

// returns nonzero if the specified keytable and key_no is currently
// waiting on a key to be set to
int KeyDrv_KeyMappingBeingSet(int keytable_index, int key_no)
{
	//return 0;
		lprintf("keydrv:  mapping being set.\n");

	return (setting_keymap_ktindex==keytable_index && setting_keymap_keyno==key_no);
}


int KeyDrv_Start(void)
{
	//return 0;
	lprintf("Starting keyboard driver...\n");
	KeyDrv_LoadKeyMappings();
	lprintf("%d,%d,%d\n", keymappings_head, 15, get_value(keymappings, &keymappings_head, 15) );

//	lprintf("Button 15 mapped to :\n",get_value(keymappings, &keymappings_head, 15));
	//memset(keytable, 0, sizeof(keytable));
	//memset(last_keytable, 1, sizeof(last_keytable));
	//memset(sdl_keysdown, 0, sizeof(sdl_keysdown));
	setting_keymap_ktindex = 0;
	
	return 0;
}

void KeyDrv_Stop(void)
{
		lprintf("keydrv:  keyboard driver stop.\n");

}

// we process the GOD cheat code here so that it's not necessary to
// have a seperate keytable[] index for each cheat code letter
int key_g=0, key_o=0, key_d=0;
SDL_Event event;


// switch button to number
//  A -> 0
//	B -> 1
//	X -> 2
// 	Y -> 3
// R1 -> 7
// R2 -> 9
// R3 -> 5
// L1 -> 6
// L2 -> 8
// L3 -> 4
// Y-axis -> Axis 1 ( up is neg) 
// X-axis -> Axis 0 ( right is pos)
// L -> 12
// U -> 13
// R -> 14
// D -> 15
// - -> 11
// + -> 10

// update the event processing
int poll_events(void)
{

	//lprintf("keydrv: polling.....\n");

	for (int i = 0; i < 2; i++) {
        if (SDL_JoystickOpen(i) == NULL) {
            SDL_Log("SDL_JoystickOpen: %s\n", SDL_GetError());
            SDL_Quit();
            return -1;
        }
	}
		char newState;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

				case SDL_JOYBUTTONUP:
					if(event.jbutton.button >=0 && event.jbutton.button <= 15)
					{
						newState = 0;
						goto prockey;
					}
					break;
				
				case SDL_JOYBUTTONDOWN:
					if(event.jbutton.button >=0 && event.jbutton.button <= 15)
					{
						if(event.jbutton.button == JOY_MINUS)
						{
							DO_QUIT = 1;
						}
						if(event.jbutton.button == JOY_L1)
						{
							int i=0;
							for(i=0; i<= keymappings_head; i++)
							{
								lprintf("(%d,%d) , ",keymappings[i].key,keymappings[i].value);
							}
							lprintf("\n");
						}
						SDL_Log("Joystick %d button %d down\n",
								event.jbutton.which, event.jbutton.button);
						newState = 1;
						if (setting_keymap_ktindex)	// for input configuration
						{
							if (event.jbutton.button == 11)
							{
								lastkeychanged = -1;
								if (setting_keymap_keyno > 0)
									reverse_keymappings[setting_keymap_ktindex].sdl_keysym[setting_keymap_keyno] = NOKEY;
							}
							else
								reverse_keymappings[setting_keymap_ktindex].sdl_keysym[setting_keymap_keyno] = event.jbutton.button;
							
							//reverse_mappings_to_mappings();
							setting_keymap_ktindex = 0;
							break;
						}
					}
					else break;
					prockey: ;
						//if ( get_value(keymappings, &keymappings_head, event.jbutton.button))//[event.key.keysym.sym])
						//{
							do_print = 1;
							keytable[get_value(keymappings, &keymappings_head, event.jbutton.button)] = newState;
							do_print = 0;
						//}
						

						set_value(sdl_keysdown, &sdl_keysdown_head, event.jbutton.button, newState);			
						//lprintf("Jump state: %d, %d \n", keytable[KCTRL], get_value(keymappings, &keymappings_head, event.jbutton.button));	
						//lprintf("Key state : %d , %d\n",get_value(sdl_keysdown, &sdl_keysdown_head, event.jbutton.button),newState);
						break;

                    break;

				break;
			}
			
	}
    
/*
char newState;
SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch( event.type ) 
		{
			case SDL_QUIT:
				crash("SDL: Got quit event!");
				break;
			
			case SDL_KEYUP:
				newState = 0;
				goto prockey;
			case SDL_KEYDOWN:
				newState = 1;
				if (setting_keymap_ktindex)	// for input configuration
				{
					if (event.key.keysym.sym==SDLK_ESCAPE)
					{
						lastkeychanged = -1;
						if (setting_keymap_keyno > 0)
							reverse_keymappings[setting_keymap_ktindex].sdl_keysym[setting_keymap_keyno] = NOKEY;
					}
					else
						reverse_keymappings[setting_keymap_ktindex].sdl_keysym[setting_keymap_keyno] = event.key.keysym.sym;
						
					reverse_mappings_to_mappings();
					setting_keymap_ktindex = 0;
					break;
				}
prockey: ;

				if (keymappings[event.key.keysym.sym])
				{
					keytable[keymappings[event.key.keysym.sym]] = newState;
				}
				
				sdl_keysdown[event.key.keysym.sym] = newState;				
				break;
			
			case SDL_MOUSEMOTION:
				mouse_x = event.motion.x;
				mouse_y = event.motion.y;
				break;
			
			case SDL_MOUSEBUTTONDOWN:
				mouse_x = event.button.x;
				mouse_y = event.button.y;
				if (event.button.button==SDL_BUTTON_LEFT)
					mouseL = 1;
				else if (event.button.button==SDL_BUTTON_RIGHT)
					mouseR = 1;
				break;
				
			case SDL_MOUSEBUTTONUP:
				mouse_x = event.button.x;
				mouse_y = event.button.y;
				if (event.button.button==SDL_BUTTON_LEFT)
					mouseL = 0;
				else if (event.button.button==SDL_BUTTON_RIGHT)
					mouseR = 0;
				break;
        }
    }
	*/
	
}
