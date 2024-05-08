import tkinter as tk
from tkinter import ttk, messagebox
import pandas as pd
import webbrowser
import serial

window = tk.Tk()
window.geometry("450x200")
window.title("車牌驗證系統")

style = ttk.Style() 
style.theme_use('clam')  # set style theme

# 加額外空白
padding_frame = ttk.Frame(window)  
padding_frame.pack(padx=30, pady=30)

# 包括兩個核心元素的 frame
main_frame = ttk.Frame(padding_frame, padding=(20,20))  
main_frame.grid(row=0, column=0)

label = ttk.Label(main_frame, text="正在驗證車牌,請稍等...", font=("Arial", 12))
label.grid(row=0, column=0, pady=(10,0))
msg_label = ttk.Label(main_frame, font=("Arial", 12))
msg_label.grid(row=1, column=0, pady=(30,0))

def check_uid():
    global ser
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        uid = line
        if uid in df['UID'].values:
            display_message(f"車牌: {uid} 匹配成功", "green")
            ser.write(b'rotate_servo')  # 打開道閘
        else:
            display_message(f"車牌: {uid} 未匹配", "red")
            open_registration_page()
    
    window.after(100, check_uid)

def display_message(message, color):
    msg_label.config(text=message, foreground=color)

def open_registration_page():
    if messagebox.askyesno("提示", "車牌未匹配, 是否需要打開註冊網頁？"):
        webbrowser.open(url)

ser = serial.Serial('COM6', 9600, timeout=1)
ser.flush()

df = pd.read_csv('Student.csv')
url = 'http://127.0.0.1/phpmyadmin/work/index.html'  # 註冊網頁url

window.after(100, check_uid)
window.mainloop()