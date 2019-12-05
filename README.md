# - SAM


A project about a fancier kind of weather station

I used some more high-tech sensors and stuff like that

Basically you can imagine SAM like this: There's a base (which looks kinda like a birds' house) that contains all the sensors. This base then reads all the data from the sensors and transmits it via radio signal to the mirror (it's similar to a smart mirror with a display built in).By the way the base is solar powered The mirror then interprets all values and displays and loggs them. You can take these values later on and copy & paste them in e.g. an Excel file which you can then use to draw graphs.

If you got any ideas for improvement or just anything you want to say to us, feel free to send a mail to aynril.play@gmail.com and i I´ll answer it.

My main problems (and how I solved them...) were the following:

Actually there were no problems with the base that couldn´t be solved right away (the base hardware was built by one team member and I designed the software and combined the two with the help of the same guy). The display was constructed and first coded by another member of my robotics team. Because of the fact that he didn´t have the base to test it, we thought that the display and logging part worked alright. But that was not the case as you might have noticed. The display worked alright at his house and so he gave it to me (the display design was created by another guy out of the team (who would´ve guessed itxD)). But as soon as I wanted to test it, I noticed a few things. First, the NRF library was not the same as mine which made the communication a bit more difficult, but after re-copy-pasting this, it worked fine (at first...). Then a lot more problems appeared and my desire to continue fell indirectly proportional to the number of problems appearing... So I had to motivate myself over and over again to complete the project. And the further I came, the more, even more unsolvable kind of problems appeared (e.g. Display problems, first it worked, then it no longer did, then it worked again but soon stopped working... idfky but I got us a new (now Liquid Crystal 4x20) display and this works.). Okay so, the display works now, but what about the radio communication? You are completely right, it also didn´t work anymore, so I fixed some stuff and optimized everything until it finally worked again. Another big issue was the theory of how our code should work, because I don´t want to miss a radio signal, but I still want to switch pages. The process ´til we got where we are right now was long and hard but finally one of my team got the answer on how to solve it (you can see this in the code as I won´t go into further details here).

I guess, I missed at least half of the problems but these are the ones that were in my head right now. Hope you learnt something while reading through this:)

So as you can see I used my summer vacation quite intensely and I´m looking forward to a lot more cool projects.

A huge thanks to all helpers. Especially to:

Julius, Mattis, Konsti, Daniel, Isi:)
