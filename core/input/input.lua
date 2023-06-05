
local input = { 
    status = 0,
    keymap = {
        KEY_A               = 65,       -- Key: A | a
        KEY_B               = 66,       -- Key: B | b
        KEY_C               = 67,       -- Key: C | c
        KEY_D               = 68,       -- Key: D | d
        KEY_E               = 69,       -- Key: E | e
        KEY_F               = 70,       -- Key: F | f
        KEY_G               = 71,       -- Key: G | g
        KEY_H               = 72,       -- Key: H | h
        KEY_I               = 73,       -- Key: I | i
        KEY_J               = 74,       -- Key: J | j
        KEY_K               = 75,       -- Key: K | k
        KEY_L               = 76,       -- Key: L | l
        KEY_M               = 77,       -- Key: M | m
        KEY_N               = 78,       -- Key: N | n
        KEY_O               = 79,       -- Key: O | o
        KEY_P               = 80,       -- Key: P | p
        KEY_Q               = 81,       -- Key: Q | q
        KEY_R               = 82,       -- Key: R | r
        KEY_S               = 83,       -- Key: S | s
        KEY_T               = 84,       -- Key: T | t
        KEY_U               = 85,       -- Key: U | u
        KEY_V               = 86,       -- Key: V | v
        KEY_W               = 87,       -- Key: W | w
        KEY_X               = 88,       -- Key: X | x
        KEY_Y               = 89,       -- Key: Y | y
        KEY_Z               = 90,       -- Key: Z | z
        KEY_SPACE           = 32,       -- Key: Space
        KEY_ESCAPE          = 256,      -- Key: Esc
        KEY_ENTER           = 257,      -- Key: Enter
        KEY_TAB             = 258,      -- Key: Tab
        KEY_BACKSPACE       = 259,      -- Key: Backspace
        KEY_INSERT          = 260,      -- Key: Ins
        KEY_DELETE          = 261,      -- Key: Del
        KEY_RIGHT           = 262,      -- Key: Cursor right
        KEY_LEFT            = 263,      -- Key: Cursor left
        KEY_DOWN            = 264,      -- Key: Cursor down
        KEY_UP              = 265,      -- Key: Cursor up
        KEY_PAGE_UP         = 266,      -- Key: Page up
        KEY_PAGE_DOWN       = 267,      -- Key: Page down
        KEY_HOME            = 268,      -- Key: Home
        KEY_END             = 269,      -- Key: End
        KEY_CAPS_LOCK       = 280,      -- Key: Caps lock
        KEY_SCROLL_LOCK     = 281,      -- Key: Scroll down
        KEY_NUM_LOCK        = 282,      -- Key: Num lock
        KEY_PRINT_SCREEN    = 283,      -- Key: Print screen
        KEY_PAUSE           = 284,      -- Key: Pause
        KEY_F1              = 290,      -- Key: F1
        KEY_F2              = 291,      -- Key: F2
        KEY_F3              = 292,      -- Key: F3
        KEY_F4              = 293,      -- Key: F4
        KEY_F5              = 294,      -- Key: F5
        KEY_F6              = 295,      -- Key: F6
        KEY_F7              = 296,      -- Key: F7
        KEY_F8              = 297,      -- Key: F8
        KEY_F9              = 298,      -- Key: F9
        KEY_F10             = 299,      -- Key: F10
        KEY_F11             = 300,      -- Key: F11
        KEY_F12             = 301,      -- Key: F12
        KEY_LEFT_SHIFT      = 340,      -- Key: Shift left
        KEY_LEFT_CONTROL    = 341,      -- Key: Control left
        KEY_LEFT_ALT        = 342,      -- Key: Alt left
        KEY_LEFT_SUPER      = 343,      -- Key: Super left
        KEY_RIGHT_SHIFT     = 344,      -- Key: Shift right
        KEY_RIGHT_CONTROL   = 345,      -- Key: Control right
        KEY_RIGHT_ALT       = 346,      -- Key: Alt right
        KEY_RIGHT_SUPER     = 347       -- Key: Super right
    }
}

function input.init()
    print("- Lua::Input::Init")
    input.status = 1
end

function input.get_key_press(keyname)
    return 1 == h_core_input_get_key_press(input.keymap[keyname])
end

return input
