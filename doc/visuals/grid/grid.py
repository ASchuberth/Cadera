from manim import *


class CreateCircle(Scene):
    def construct(self):
        circle = Circle()
        circle.set_fill(PINK, opacity=0.5)
        self.play(Create(circle))

class CreateGrid(Scene):

    def construct(self):

        line = Line()
        line.set_length(2)
        line.set_color(WHITE)
        line.set_angle(-PI/2)
        #line.shift(LEFT * .2)


        line2 = Line()
        line2.set_length(2)
        line2.set_color(WHITE)
        line2.set_angle(0)
        #line2.shift(UP)
        
        self.play(Create(line))
        #self.play(Rotate(line,-PI/2, about_point=ORIGIN))
        self.play(Create(line2))
        self.play(FadeOut(line))
        self.play(FadeOut(line2))
        

