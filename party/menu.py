import os,sys
import pygame
from pygame.locals import *
import pygame._view

class Image(pygame.sprite.Sprite):
    
    def __init__(self, image_file, location):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.image.load(image_file)
        self.rect = self.image.get_rect()
        self.rect.left, self.rect.top = location

class Button(pygame.sprite.Sprite):
    
    def __init__(self, text, pos, screen, command, color):
        self.text = text
        self.pos = pos
        self.screen = screen
        self.command = command
        self.color = color
        
        self.set_rect()
        self.draw()
            
    def draw(self):
        self.set_rend()
        self.screen.blit(self.rend, self.rect)
        
    def set_rend(self):
        print self.get_color()
        self.rend = menu_font.render(self.text, True, self.get_color())
        
    def get_color(self):
        if len(self.color) == 2:
            if self.color[0] == "red":
                return(200, 200, 0)
            else:
                return (200, 200, 0)
        elif self.color[0] == "red":
            return (100, 0, 0)
        elif self.color[0] == "green":
            return (0, 100, 0)        
        
    def set_rect(self):
        self.set_rend()
        self.rect = self.rend.get_rect()
        self.rect.topleft = self.pos

    def action(self):
        if(self.command == "quit"):
            pygame.quit()
            sys.exit()
        if(self.command == "start"):
            os.system("gra.exe")
            pygame.quit()
            sys.exit()            

class PyManMain:

    def __init__(self, width=1000, height=700):
        self.width = width
        self.height = height
        self.screen = pygame.display.set_mode((self.width, self.height))
        self.background = pygame.image.load("background.bmp")
        self.screen.blit(self.background, self.screen.get_rect())
        pygame.display.update

        self.Buttons = []
        self.Buttons.append(Button("WYJDZ", (600,self.height / 2 - (50)), self.screen, "quit", ["red"]))
        self.Buttons.append(Button("WEJDZ", (100 ,self.height / 2 - (50)), self.screen, "start", ["green"]))

    def MainLoop(self):            
        while 1:
            pos = pygame.mouse.get_pos()            
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                if event.type == pygame.MOUSEBUTTONDOWN:
                    for button in self.Buttons:
                        rect = button.rend.get_rect()
                        print rect[3]
                        if (pos[0] > button.pos[0]) and pos[0] < (button.pos[0] + rect[2]):
                            if (pos[1] > button.pos[1]) and pos[1] < (button.pos[1] + rect[3]):
                                button.action()
                                

                
            for button in self.Buttons:
                rect = button.rend.get_rect()
                n = len(button.color)
                if (pos[0] > button.pos[0]) and pos[0] < (button.pos[0] + rect[2]):
                    if (pos[1] > button.pos[1]) and pos[1] < (button.pos[1] + rect[3]):
                        if n == 1:
                            button.color.append("bright")
                    elif n > 1:
                        button.color.pop(1)
                elif n > 1:
                    button.color.pop(1)

                button.draw()
                
if __name__ == "__main__":
    pygame.init()
    menu_font = pygame.font.Font(None, 140)
    MainWindow = PyManMain()
    MainWindow.MainLoop()
