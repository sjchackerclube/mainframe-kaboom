#!/usr/bin/python

"""
GUI

Requirements:
 - Python 2.6, 2.7 or 3.3
 - PySDL2
"""

import random
import datetime
import atexit

import sdl2
import sdl2.ext
import sdl2.sdlmixer

def rect_has_point(rect, point):
    """Test whether a point (x, y) is inside a rectangle (x, y, w, h)."""
    rx, ry, rw, rh = rect
    px, py = point
    return rx <= px < (rx + rw) and ry <= py < (ry + rh)

class GUI:
    bg = 0x00002A
    width, height = 1000, 750
    bomb_time = datetime.timedelta(minutes=15)
    nchannels = 16
    delay = sdl2.timer.SDL_Delay
    BLACK = 0x000000
    RED   = 0xFF0000
    GREEN = 0x00FF00
    BLUE  = 0x0000FF
    LEFT   = sdl2.SDL_BUTTON_LEFT
    MIDDLE = sdl2.SDL_BUTTON_MIDDLE
    RIGHT  = sdl2.SDL_BUTTON_RIGHT

    def __init__(self):
        self.init()
        atexit.register(self.quit)
        self.create_window()
        self.load_fonts()
        self.clear_window()
        self.load_samples()

        self.active = True
        self.toggle_active()
        self.draw_players()

    def init(self):
        sdl2.ext.init()
        sdl2.sdlmixer.Mix_Init(
          sdl2.sdlmixer.MIX_INIT_OGG | sdl2.sdlmixer.MIX_INIT_MP3
        )
        sdl2.sdlmixer.Mix_OpenAudio(
          sdl2.sdlmixer.MIX_DEFAULT_FREQUENCY,
          sdl2.sdlmixer.MIX_DEFAULT_FORMAT,
          2, # 1: mono, 2: stereo
          1024, # chunksize
        )
        sdl2.sdlmixer.Mix_AllocateChannels(self.nchannels)
        self.channel = 0

    def quit(self):
        for key in self.samples:
            sdl2.sdlmixer.Mix_FreeChunk(self.samples[key])
        sdl2.sdlmixer.Mix_CloseAudio()
        sdl2.sdlmixer.Mix_Quit()
        sdl2.ext.quit()

    def create_window(self):
        # Double buffer may cause flickering.
        sdl2.SDL_GL_SetAttribute(sdl2.SDL_GL_DOUBLEBUFFER, 0)
        win = sdl2.ext.Window(
            "Capture the Flag", size=(self.width, self.height),
            flags=sdl2.SDL_WINDOW_FULLSCREEN
        )
        win.show()
        sdl2.SDL_SetHint(sdl2.SDL_HINT_RENDER_SCALE_QUALITY, b"linear")
        rc = sdl2.ext.RenderContext(win)
        sdl2.SDL_RenderSetLogicalSize(rc.renderer, self.width, self.height)
        rc.clear(0)
        self.win = win # Window
        self.rc = rc # RenderContext
        self.buttons = {} # {rect: (callback, args), ...}

    def load_fonts(self):
        surf = sdl2.sdlimage.IMG_Load(b"media/font/7sledfont_half.png").contents
        ledfont = sdl2.ext.BitmapFont(surf, (70, 100), ["01234-", "56789:"])
        # font source: http://www.dafont.com/franchise.font
        fm = sdl2.ext.FontManager("media/font/franchise.ttf", size=25)
        sfac = sdl2.ext.SpriteFactory(
          sdl2.ext.TEXTURE, renderer=self.rc, fontmanager=fm
        )
        self.ledfont = ledfont # BitmapFont
        self.sfac = sfac # SpriteFactory

    def load_samples(self):
        self.samples = dict(
            cs_bomb_planted=sdl2.sdlmixer.Mix_LoadWAV(b"media/sound/cs-bomb-planted.ogg")
        )

    def play_sample(self, sample, volume=1):
        volume = int(sdl2.sdlmixer.MIX_MAX_VOLUME * volume)
        sdl2.sdlmixer.Mix_Volume(self.channel, volume)
        sdl2.sdlmixer.Mix_PlayChannel(self.channel, self.samples[sample], 0)
        self.channel = (self.channel + 1) % self.nchannels

    def clear_window(self):
        self.rc.fill((0, 0, self.width, self.height), self.bg)
        surf = sdl2.sdlimage.IMG_Load(b"media/image/layout.png").contents
        txtr = self.sfac.from_surface(surf)
        rect = (0, 0, self.width, self.height)
        self.rc.copy(txtr, rect, rect)

    def draw_toggle_button(self):
        if self.active:
            text = "DESATIVAR"
        else:
            text = "ATIVAR"
        button = self.sfac.from_text(
          text, size=40,
          color=self.RED, bg_color=self.BLACK
        )
        w, h = button.size
        h = int(h * 0.82) # Remove font's bottom margin.
        x, y = (self.width - w) // 2, 190
        border = 3
        margin = 7
        offset = border + margin
        outrect = (x - offset, y - offset, w + offset * 2, h + offset * 2)
        inrect  = (x - margin, y - margin, w + margin * 2, h + margin * 2)
        self.rc.fill((0, y - 10, self.width, h + 20), self.bg)
        self.rc.fill(outrect, self.RED)
        self.rc.fill(inrect, self.BLACK)
        self.rc.copy(button, (0, 0, w, h), (x, y, w, h))
        for key in list(self.buttons.keys()):
            if self.buttons[key][0] == self.toggle_active:
                del self.buttons[key]
        self.link_button(outrect, self.LEFT, self.toggle_active)

    def draw_players(self):
        positions = [
          (185, 270), (185, 434), (185, 598),
          (528, 270), (528, 434), (528, 598)
        ]
        w, h = 287, 104
        m = 15
        bar_w, bar_h = w - m * 2, (h - m * 2) // 2 - 5
        progress_rects = [[x + m, y + m, bar_w, bar_h] for x, y in positions]
        time_rects = [[x + m, y + m + bar_h + 10, bar_w, bar_h] for x, y in positions]
        for rect in progress_rects:
            self.rc.draw_rect(rect, self.GREEN)
            p = random.random()
            if rect[0] > 500:
                rect[0] += int(rect[2] * (1 - p))
            rect[2] = int(rect[2] * p)
            self.rc.fill(rect, self.GREEN)
            step = bar_w // 10
            for i in range(9):
                self.rc.fill((rect[0] + step * (i + 1) - 1, rect[1] + 1, 2, rect[3] - 2), self.bg)
        for rect in time_rects:
            self.rc.draw_rect(rect, self.RED)
            t = random.random()
            if rect[0] > 500:
                rect[0] += int(rect[2] * (1 - t))
            rect[2] = int(rect[2] * t)
            self.rc.fill(rect, self.RED)
            step = bar_w // 10
            for i in range(9):
                self.rc.fill((rect[0] + step * (i + 1) - 1, rect[1] + 1, 2, rect[3] - 2), self.bg)

    def draw_clock(self, clock_str="--:--:-"):
        assert len(clock_str) == 7
        assert self.ledfont.can_render(clock_str)
        surf = self.ledfont.render(clock_str).surface
        clock = self.sfac.from_surface(surf, True)
        w, h = clock.size
        self.rc.copy(clock, (0, 0, w, h), ((self.width - w) // 2, 40, w, h))

    def link_button(self, rect, button, callback, args=[]):
        """Set a callback for clicks on a rectangular region."""
        self.buttons[rect, button] = callback, args

    def mouse_down(self, button):
        """Call the appropriate callback upon mouse click."""
        point = button.x, button.y
        for rect, btn in self.buttons:
            if btn == button.button and rect_has_point(rect, point):
                callback, args = self.buttons[rect, btn]
                break
        else:
            return
        callback(*args)

    def key_down(self, keycode):
        """Handle keypress.

        See possible `keycode` values at http://wiki.libsdl.org/SDL_Keycode.
        """
        if keycode == sdl2.keycode.SDLK_q:
            self.running = False

    def toggle_active(self):
        if self.active:
            self.draw_clock()
        else:
            self.start_time = datetime.datetime.now()
            self.play_sample('cs_bomb_planted')
        self.active = not self.active
        self.draw_toggle_button()

    def main_loop(self):
        self.running = True
        while self.running:
            for event in sdl2.ext.get_events():
                if event.type == sdl2.events.SDL_QUIT:
                    self.running = False
                elif event.type == sdl2.events.SDL_MOUSEBUTTONDOWN:
                    self.mouse_down(event.button)
                elif event.type == sdl2.events.SDL_KEYDOWN:
                    self.key_down(event.key.keysym.sym)
            if self.active:
                delta = datetime.datetime.now() - self.start_time
                remaining = self.bomb_time - delta
                mins, secs = divmod(remaining.seconds, 60)
                dsecs = int(remaining.microseconds / 1e5)
                if 0 <= dsecs < 5:
                    sep = ' '
                else:
                    sep = ':'
                s = "{1:02}{0}{2:02}{0}{3}".format(sep, mins, secs, dsecs)
                self.draw_clock(s)
            self.rc.present()
            self.delay(10)

if __name__ == "__main__":
    gui = GUI()
    gui.main_loop()
