from tkinter import *
 
window = Tk()
window.title("Tkinter goes BRRRR")
window.geometry('450x600')
lbl = Label(window, text="I hate tkinter", font=("Arial Bold", 50))
lbl.grid(column=0, row=0)

txt = Entry(window,width=10)
txt.grid(column=0, row=2)

def click():
    lbl.configure(text="click!!!")

btn = Button(window, text="I am button", bg="grey", fg="darkred", font = ("Arial Bold", 30), command = click)
btn.grid(column=0, row=1)


window.mainloop()
